PROG= motion
SRCS= motion.c mpu6500.c

SDKSRCS= drivers_nrf/twi_master/twi_hw_master.c

CFLAGS+= -I$(abspath ../../config) -I.

DEBUG_LOG= rtt

include ../../build.mk
