all:
	avr-gcc -Wall --std=c89 -Os -DF_CPU=8000000 -mmcu=attiny85 -o main.o main.c
	avr-objcopy -j .text -j .data -O ihex main.o main.hex

flash:
	avrdude -c usbtiny -p t85 -U flash:w:main.hex:i
