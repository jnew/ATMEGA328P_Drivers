### AVR ATMEGA328P Drivers

To better understand firmware driver design and implementation I am creating from (mostly) scratch a set of drivers for the ATMEGA328P microcontroller, as well as a small program to demonstrate functionality.

Toolchain: WinAVR

I will attempt to use minimal library assistance, but I will be using simple register/interrupt vector alias libraries like avr/io.h, and standard library functions like string compare.

Current Functionality:

* Interrupt driven USART TX/RX.
* Configuration and polling driven read of ADC channels.
