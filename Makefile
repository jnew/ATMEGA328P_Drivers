OUT = blink
PORT = COM5

all: no_flash flash
no_flash: elf hex
elf:
	avr-gcc -Os -DF_CPU=16000000L -mmcu=atmega328p -o $(OUT).elf blink.c usart.h usart.c
hex:
	avr-objcopy -j .text -j .data -O ihex $(OUT).elf $(OUT).hex
flash:
	avrdude -V -p ATMEGA328P -P $(PORT) -b 115200 -U flash:w:$(OUT).hex -c arduino
clean:
	rm -rf $(OUT).elf usart.o $(OUT).hex