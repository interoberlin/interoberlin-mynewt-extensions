PROG= bridge-uart
SRCS= bridge-uart.c

#SDKINCDIRS= libraries/gpiote
SDKSRCS= drivers_nrf/uart/app_uart_fifo.c libraries/gpiote/app_gpiote.c libraries/fifo/app_fifo.c

CFLAGS+= -I$(abspath ../../config) -I.

DEBUG_LOG= rtt

include ../../build.mk
