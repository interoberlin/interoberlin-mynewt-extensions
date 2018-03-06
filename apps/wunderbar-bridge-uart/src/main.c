
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
