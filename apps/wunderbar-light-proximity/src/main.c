
void
main(void)
{
        nrf_gpio_cfg_input(TCS37717_INT_PIN, GPIO_PIN_CNF_PULL_Pullup);
        nrf_gpio_cfg_output(WLED_CTRL_PIN);

        twi_master_init();
        disable_i2c();

        simble_init("RGB/Proximity");
        proximity_ctx.sampling_period = DEFAULT_SAMPLING_PERIOD;
        rgb_ctx.sampling_period = DEFAULT_SAMPLING_PERIOD;
        //Set the timer parameters and initialize it.
        struct rtc_ctx rtc_ctx = {
                .rtc_x[NOTIF_TIMER_ID_PROX] = {
                        .type = PERIODIC,
                        .period = proximity_ctx.sampling_period,
                        .enabled = false,
                        .cb = proximity_notif_timer_cb,
                },
                .rtc_x[NOTIF_TIMER_ID_RGB] = {
                        .type = PERIODIC,
                        .period = rgb_ctx.sampling_period,
                        .enabled = false,
                        .cb = rgb_notif_timer_cb,
                }
        };
        batt_serv_init(&rtc_ctx);
        rtc_init(&rtc_ctx);
        ind_init();
        proximity_init(&proximity_ctx);
        rgb_init(&rgb_ctx);
        simble_adv_start();

        simble_process_event_loop();
}
