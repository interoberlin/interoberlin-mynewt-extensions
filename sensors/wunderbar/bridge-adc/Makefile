PROG= bridge-adc
SRCS= bridge-adc.c adc121c02.c

SDKSRCS= drivers_nrf/twi_master/twi_hw_master.c

CFLAGS+= -I$(abspath ../../config) -I.

DEBUG_LOG= rtt

include ../../build.mk
