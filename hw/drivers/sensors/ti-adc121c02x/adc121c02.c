/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 relayr (iThings4U GmbH)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <sys/types.h>
#include <string.h>

#include <twi_master.h>

#include "adc121c02.h"

#define ADC121C02 (0x55 << 1)

#define ADC121C02_RESULT 0
#define ADC121C02_CONFIG 2
#define ADC121C02_CONFIG_CYCLE(x) ((x) << 5)


static void
adc121c02_write_register(uint8_t addr, void *data, size_t len)
{
        uint8_t packet[len + 1];
        packet[0] = addr;
        memcpy(&packet[1], data, len);

        twi_master_transfer(ADC121C02, packet, sizeof(packet), TWI_ISSUE_STOP);
}

static void
adc121c02_read_register(uint8_t addr, void *data, size_t len)
{
        uint8_t cmd[] = { addr };

        twi_master_transfer(ADC121C02, cmd, sizeof(cmd), TWI_DONT_ISSUE_STOP);
        twi_master_transfer(ADC121C02 | TWI_READ_BIT, data, len, TWI_ISSUE_STOP);
}

void
adc121c02_init(void)
{
        uint8_t data[] = {
                ADC121C02_CONFIG_CYCLE(7)
        };
        adc121c02_write_register(ADC121C02_CONFIG, data, sizeof(data));
}

void
adc121c02_stop(void)
{
        uint8_t data1[] = {
                0
        };
        adc121c02_write_register(ADC121C02_CONFIG, data1, sizeof(data1));
}

uint16_t
adc121c02_sample(void)
{
        uint16_t val;

        adc121c02_read_register(ADC121C02_CONFIG, &val, sizeof(val));
        return (val);
}
