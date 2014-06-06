/* This is a simple adc driver for ATMEGA328P.
	Author: John New
	Last Revision: 6-5-14
*/

#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <avr/io.h>

void adc_enable();

void adc_disable();

void adc_select_channel(uint8_t channel_number);

void adc_enable_free_running();

uint8_t adc_read_value_8bit();

uint16_t adc_read_value_16bit();

#endif