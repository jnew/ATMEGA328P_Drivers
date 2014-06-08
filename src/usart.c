#include "../lib/usart.h"

void usart0_config()
{
    // set baud, hard coded as 19200 for now
    UBRR0H = (uint8_t)(51>>8);
    UBRR0L = (uint8_t)51;
    // 8 bits, 1 stop;
    UCSR0C = 0x06;

    // setup buffers
    usart_driver.send_buffer_head = 0;
    usart_driver.send_buffer_tail = 0;
    usart_driver.recv_buffer_head = 0;
    usart_driver.recv_buffer_tail = 0;
}

void usart0_enable_trans()
{
    // enable trans
    UCSR0B |= 0x08;
}

void usart0_disable_trans()
{
    // disable trans
    UCSR0B &= ~0x08;
}

void usart0_enable_recv()
{
    // enable recv
    UCSR0B |= 0x10;

    // enable RX complete interrupt
    UCSR0B |= (1 << RXCIE0);
}

void usart0_disable_recv()
{
    // disable recv
    UCSR0B &= ~0x10;

    // disable RX complete interrupt
    UCSR0B &= ~(1 << RXCIE0);
}

void usart0_send_byte(uint8_t data)
{
    usart0_send_string(&data, 1);
}

void usart0_send_string(uint8_t *data, uint8_t string_length)
{
    // add to circular buffer
    uint8_t i = 0;
    for (i; i < string_length; i++) {
        usart_driver.send_buffer[usart_driver.send_buffer_head] = data[i];
        usart_driver.send_buffer_head++;
    }

    // enable UART ready interrupt
    UCSR0B |= (1 << UDRIE0);
}

uint8_t usart0_recv_byte(uint8_t *data)
{
    if(usart_driver.recv_buffer_head == usart_driver.recv_buffer_tail) {
        return 0; // empty
    } else {
        data[0] = usart_driver.recv_buffer[usart_driver.recv_buffer_tail++];
        return 1; // got one byte
    }
}

uint8_t usart0_recv_line(uint8_t *data)
{
    uint8_t i = usart_driver.recv_buffer_tail;
    uint8_t line_index = 0;
    if(usart_driver.lines_in_buffer > 0) {
        // work through the buffer from tail to head looking for delimiters
        for(i; usart_driver.recv_buffer_head != i; i++) {
            if(usart_driver.recv_buffer[i] == '\r') {
                for(usart_driver.recv_buffer_tail; usart_driver.recv_buffer_tail < i; usart_driver.recv_buffer_tail++) {
                    data[line_index++] = usart_driver.recv_buffer[usart_driver.recv_buffer_tail];
                }
                usart_driver.recv_buffer_tail++;
                data[line_index] = '\0'; // swap line break for null terminator
                return line_index; // got a line, here's how long
                usart_driver.lines_in_buffer--;
            }
        }
    }
    return 0; // no finished lines
}

void USART_TX_vect(void)
{

}

void USART_RX_vect(void)
{
    uint8_t data = UDR0;
    if(data == '\r') {
        usart_driver.lines_in_buffer++;
    }
    usart_driver.recv_buffer[usart_driver.recv_buffer_head++] = data;
    usart0_send_byte(data);
}

void USART_UDRE_vect(void)
{
    if(usart_driver.send_buffer_head == usart_driver.send_buffer_tail) {
        // done, disable interrupt
        UCSR0B &= ~(1 << UDRIE0);
    } else {
        // there is another frame to write, write it
        UDR0 = usart_driver.send_buffer[usart_driver.send_buffer_tail++];
    }
}