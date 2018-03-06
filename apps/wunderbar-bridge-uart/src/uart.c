#include <stdlib.h>
#include <string.h>

#include <app_uart.h>

#include "simble.h"
#include "indicator.h"



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

