#include "adc.h"

void adc_enable()
{
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); // prescaler
    ADMUX |= ((1 << REFS0)); // arduino's 5v vref
    ADMUX |= (1 << ADLAR); // left adjust
    ADCSRA |= (1 << ADEN); // set adc on
}

void adc_disable()
{
    // set adc off
    ADCSRA &= ~(1 << ADEN);
}

void adc_select_channel(uint8_t channel_number)
{
    if((channel_number > 5) || (channel_number < 0)) // out of range
        return;
    // its simple here since the 4 least sig bits of ADMUX just represent the channel
    ADMUX |= (channel_number);
}

void adc_enable_free_running()
{
    // enable adc complete interrupt and have it be the start condition for next sample
    ADCSRA |= ((1 << ADATE) | (1 << ADIE));

    // 128 prescaler
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));

    // start sampling
    ADCSRA |= (1 << ADSC);
}

uint8_t adc_read_value_8bit()
{
    // start sampling
    ADCSRA |= (1 << ADSC);

    while(bit_is_set(ADCSRA, ADSC));

    uint8_t data = ADCH;
    // 8-bit resolution, left adjusted
    return data;
}

uint16_t adc_read_value_16bit()
{
    // start sampling
    ADCSRA |= (1 << ADSC);

    while(bit_is_set(ADCSRA, ADSC));

    uint16_t data = ADCL;
    data |= (ADCH << 2);
    // 10-bit resolution, left adjusted
    return data;
}