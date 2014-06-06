#include "usart.h"

volatile uint32_t count;

void TIMER0_OVF_vect(void) __attribute__ ((signal));

void TIMER0_OVF_vect(void)
{
    count++;
}

uint8_t string_compare(uint8_t *arg1, uint8_t *arg2, uint8_t length)
{
    int i;
    for(i = 0; i < length; i++) {
        if(arg1[i] != arg2[i]) {
            return 0;
        }
    }
    return 1;
}

void toggle_led(uint8_t *led_state)
{
    if(*led_state == 0) {	//set led on
        *(uint8_t *)(0x25) |= (1<<5);
        *led_state = 1;
    } else {					//set led off
        *(uint8_t *)(0x25) &= ~(1<<5);
        *led_state = 0;
    }
}

int main(void)
{
    uint8_t led_state = 0;
    uint8_t command[50];
    uint8_t command_length;

    //set DDRB so Pin B5 is output
    *(uint8_t *)(0x24) |= (1<<5);

    //set up timer
    *(uint8_t *)(0x44) = (0x00);
    *(uint8_t *)(0x45) |= (0x01);

    //global interrupt enable
    asm("sei"::);

    usart0_config();
    usart0_enable_trans();
    usart0_enable_recv();

    usart0_send_string("STARTUP COMPLETE\r\n# ", 20);

    while(1) {
        command_length = usart0_recv_line(command);
        if(command_length != 0) {
            usart0_send_byte('\n');
            if(string_compare(command, "BLINK_OFF", 9)) {
                usart0_send_string("Stopping timer0. \r\n", 19);
                // disable timer0 int
                *(uint8_t *)(0x6E) &= ~(0x01);
            } else if(string_compare(command, "BLINK_ON", 8)) {
                usart0_send_string("Starting timer0. \r\n", 19);
                // enable timer0 int
                *(uint8_t *)(0x6E) |= (0x01);
            } else if(string_compare(command, "THE_TRUTH", 9)) {
                usart0_send_string("John New is the best developer in the world. \r\n", 47);
            } else if(string_compare(command, "TOGGLE_LED", 10)) {
                toggle_led(&led_state);
                usart0_send_string("LED Toggled. \r\n", 15);
            } else {
                usart0_send_string("Invalid command. \r\n", 19);
            }
            usart0_send_string("# ", 2);
        } else if(count >= 62500) {
            toggle_led(&led_state);
            count = 0;
        }
    }

    return 0;
}

