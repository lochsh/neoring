#include <stdint.h>
#include <avr/io.h>
#include <avr/eeprom.h>

#include "leds.h"
#include "modes.h"

int main() {
    uint8_t mode;

    /* Set up GPIOs: D0 and D1 outputs, D2 input */
    DDRB  = (1<<0) | (1<<1) | (0<<2);
    PORTB = 0;

    /* Read last mode from EEPROM, increment and store it */
    mode = eeprom_read_byte((uint8_t*)0);
    mode += 1;
    if(mode >= modes_count) {
        mode = 0;
    }
    eeprom_write_byte((uint8_t*)0, mode);

    /* Execute that mode forever */
    while(1) {
        modes_list[mode]();
    }
}
