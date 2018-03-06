#include <stdlib.h>
#include <string.h>
#include <nrf_delay.h>

#include "simble.h"
#include "indicator.h"
#include "protocol.h"
#include "batt_serv.h"
#include "rtc.h"


void
main(void)
{
	simble_init("IR transmitter");
	protocol_init(&nec, IR_PIN_OUT, &rtc_ctx);
	ind_init();
	batt_serv_init(&rtc_ctx);
	rtc_init(&rtc_ctx);
	ir_init(&ir_ctx);
	simble_adv_start();
	simble_process_event_loop();
}
