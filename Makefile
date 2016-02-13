SOURCES = $(wildcard *.c)
CFLAGS = -Wall -Wextra --std=c99 -Os -DF_CPU=8000000 -mmcu=attiny85

OBJS = $(SOURCES:.c=.o)

all: hex

.c.o:
	avr-gcc $(CFLAGS) -c $< -o $@

elf: $(OBJS)
	avr-gcc $(CFLAGS) -o main.elf $(OBJS)

hex: elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

flash: hex
	avrdude -c usbtiny -p t85 -U flash:w:main.hex:i
