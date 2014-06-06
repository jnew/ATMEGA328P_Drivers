/* This is a simple usart driver for ATMEGA328P. 
	Author: John New
	Last Revision: 6-5-14
*/

#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <avr/io.h>

// This struct allows interrupt driver multi-frame transfers.
typedef struct
{
	volatile uint8_t send_buffer[256];
	volatile uint8_t recv_buffer[256];
	volatile uint8_t send_buffer_head;
	volatile uint8_t send_buffer_tail;
	volatile uint8_t recv_buffer_head;
	volatile uint8_t recv_buffer_tail;
} usart;

usart usart_driver;

// Configure as 19200 BAUD, 8 data bits, 1 stop bit.
void usart0_config();

// Enable TX.
void usart0_enable_trans();

// Disable TX.
void usart0_disable_trans();

// Enable RX.
void usart0_enable_recv();

// Disable RX.
void usart0_disable_recv();

// Send one byte.
void usart0_send_byte(uint8_t data);

// Send a string of variable length.
void usart0_send_string(uint8_t *data, uint8_t string_length);

// Pull one byte out of the recieve buffer.
// Returns number of bytes read.
uint8_t usart0_recv_byte(uint8_t *data);

// Pull one line, terminated by a \r, out of the recieve buffer.
// Returns number of bytes read.
uint8_t usart0_recv_line(uint8_t *data);

// Transmit complete interrupt handler.
void USART_TX_vect(void) __attribute__ ((signal));

// Receive complete interrupt handler.
void USART_RX_vect(void) __attribute__ ((signal));

// USART ready interrupt handler.
void USART_UDRE_vect(void) __attribute__ ((signal));


#endif