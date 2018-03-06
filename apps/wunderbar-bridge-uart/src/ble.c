
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
