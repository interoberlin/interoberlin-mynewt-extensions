#include <stdlib.h>
#include <string.h>

#include <app_uart.h>

#include "simble.h"
#include "indicator.h"


enum {
        UART_PIN_RX = 15,
        UART_PIN_TX = 16,
};


enum {
        RELAYR_UUID_UART_RX_CHAR = 0x2370,
        RELAYR_UUID_UART_TX_CHAR = 0x2371,
        RELAYR_UUID_UART_CONFIG_BAUD = 0x2372,
};


struct bridge_uart_ctx {
        struct service_desc;
        struct char_desc uart_rx;
        struct char_desc uart_tx;
        struct char_desc uart_config_baud;
        uint32_t baud;
        int indication_active;
        size_t indicate_buf_len;
        uint8_t indicate_buf[23];
};

static struct bridge_uart_ctx bridge_uart_ctx;

static uint8_t uart_rx_buf[128];
static uint8_t uart_tx_buf[128];
static uint16_t uart_id;


static void
uart_evt_handler(app_uart_evt_t *uart_evt)
{
        if (uart_evt->evt_type != APP_UART_DATA_READY)
                return;
        /* sd_nvic_SetPendingIRQ(SD_EVT_IRQn); */
}

static void
uart_init(uint32_t baud_rate, bool use_parity)
{
        uint32_t baud_rate_reg;

        /**
         * Analysis of datasheet baud rate register constants:
         *
         * Residuals:
         * Min      1Q  Median      3Q     Max
         * -2252.1 -1345.3   393.8  1135.5  1917.3
         *
         * Coefficients:
         * Estimate Std. Error   t value Pr(>|t|)
         * (Intercept) 1.890e+02  4.360e+02 4.340e-01    0.671
         * V2          2.684e+02  1.175e-03 2.284e+05   <2e-16 ***
         *
         *
         * We approximate the non-integer coefficient using these
         * binary fractions.  This gets us to typically <50ppm of the
         * datasheet value.
         */
        baud_rate_reg = baud_rate * 268;
        baud_rate_reg += baud_rate >> 1;
        baud_rate_reg -= baud_rate >> 4;
        baud_rate_reg -= baud_rate >> 8;
        baud_rate_reg -= baud_rate >> 12;
        baud_rate_reg += 189;

        app_uart_comm_params_t uart_comm_params = {
                .rx_pin_no = UART_PIN_RX,
                .tx_pin_no = UART_PIN_TX,
                .flow_control = APP_UART_FLOW_CONTROL_DISABLED,
                .use_parity = use_parity,
                .baud_rate = baud_rate_reg,
        };
        app_uart_buffers_t uart_buffers = {
                .rx_buf = uart_rx_buf,
                .rx_buf_size = sizeof(uart_rx_buf),
                .tx_buf = uart_tx_buf,
                .tx_buf_size = sizeof(uart_tx_buf),
        };

        app_uart_init(&uart_comm_params,
                      &uart_buffers,
                      uart_evt_handler,
                      APP_IRQ_PRIORITY_HIGH,
                      &uart_id);

        simble_srv_char_update(&bridge_uart_ctx.uart_config_baud, &baud_rate);
}

static void
bridge_indicate_data(struct bridge_uart_ctx *ctx, int fill_buffer)
{
        if (ctx->indication_active)
                return;

        if (fill_buffer) {
                ctx->indicate_buf_len = 0;
                while (ctx->indicate_buf_len < sizeof(ctx->indicate_buf)) {
                        if (app_uart_get(&ctx->indicate_buf[ctx->indicate_buf_len]) != NRF_SUCCESS)
                                break;
                        ++ctx->indicate_buf_len;
                }
        }

        if (ctx->indicate_buf_len == 0)
                return;

        if (simble_srv_char_notify(&ctx->uart_rx, 1, ctx->indicate_buf_len, ctx->indicate_buf) == NRF_SUCCESS)
                ctx->indication_active = 1;
}


static void
bridge_uart_notify_status_cb(struct service_desc *s, struct char_desc *c, const int8_t status)
{
        struct bridge_uart_ctx *ctx = (struct bridge_uart_ctx *)s;

        if (status & BLE_GATT_HVX_NOTIFICATION) {
                bridge_indicate_data(ctx, 1);
        }
}

static void
bridge_uart_indicated_cb(struct service_desc *s, struct char_desc *c, int success)
{
        struct bridge_uart_ctx *ctx = (void *)s;

        ctx->indication_active = 0;
        bridge_indicate_data(ctx, success);
}

static void
bridge_uart_tx_write_cb(struct service_desc *s, struct char_desc *c, const void *val, uint16_t len, uint16_t offset)
{
        if (offset != 0)
                return;

        const uint8_t *buf = val;
        for (size_t i = 0; i < len; ++i)
                app_uart_put(buf[i]);
}

static void
bridge_uart_baud_write_cb(struct service_desc *s, struct char_desc *c, const void *val, uint16_t len, uint16_t offset)
{
        uint32_t baud;

        if (offset != 0 || len != sizeof(baud))
                return;

        memcpy(&baud, val, len);
        if (baud < 300 || baud > 1000000)
                return;

        uart_init(baud, 0);
}


static void
bridge_uart_event_loop(void)
{
        bridge_indicate_data(&bridge_uart_ctx, 1);
}

static void
bridge_uart_init(struct bridge_uart_ctx *ctx)
{
        simble_srv_init(ctx, simble_get_vendor_uuid_class(), VENDOR_UUID_SENSOR_SERVICE);

        simble_srv_char_add(ctx, &ctx->uart_rx,
                            simble_get_vendor_uuid_class(), RELAYR_UUID_UART_RX_CHAR,
                            u8"RX",
                            sizeof(ctx->indicate_buf));
        ctx->uart_rx.indicate = 1;
        ctx->uart_rx.notify_status_cb = bridge_uart_notify_status_cb;
        ctx->uart_rx.indicated_cb = bridge_uart_indicated_cb;

        simble_srv_char_add(ctx, &ctx->uart_tx,
                            simble_get_vendor_uuid_class(), RELAYR_UUID_UART_TX_CHAR,
                            u8"TX",
                            sizeof(ctx->indicate_buf) /* XXX own size */);
        ctx->uart_tx.write_cb = bridge_uart_tx_write_cb;

        simble_srv_char_add(ctx, &ctx->uart_config_baud,
                            simble_get_vendor_uuid_class(), RELAYR_UUID_UART_CONFIG_BAUD,
                            u8"baud rate",
                            sizeof(uint32_t));
        ctx->uart_config_baud.write_cb = bridge_uart_baud_write_cb;

        simble_srv_register(ctx);
}


void
main(void)
{
        simble_init("Bridge-UART");
        bridge_uart_init(&bridge_uart_ctx);
        ind_init();
        uart_init(19200, 0);
        simble_set_process_event_loop_cb(bridge_uart_event_loop);

        simble_adv_start();
        simble_process_event_loop();
}
