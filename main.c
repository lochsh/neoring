#include <stdint.h>
#include <avr/io.h>

/* Store LED state */
typedef struct {
    uint8_t G, R, B;
} LED;
volatile LED LEDS[16];

/* Send out the LED data */
void draw_leds() {
    /* Let the compiler pick registers for a, b, and c */
    uint8_t a = 0, b = 0, c = 0;

    __asm__ __volatile__ (
        /*
         * %0 - big counter, reset timer / number of bytes sent
         * %1 - small counter, number of bits sent in byte
         * %2 - value, current value to send
         */
        /* RESET signal */
        "    cbi  24, 0         \n\t" /* 2   cycles */
        "    ldi  %0, 140       \n\t" /* 1   cycles */
        "l0: dec  %0            \n\t" /* 1   cycles */
        "    brne l0            \n\t" /* 1/2 cycles */

        /* Read out 16 LEDs with 3 bytes each */
        "    ldi %0,48          \n\t" /* 1   cycles */

        /* Read the current byte to send */
        "l1: ld  %2, %a[LEDS]+  \n\t" /* 2   cycles */
        "    ldi %1, 8          \n\t" /* 1   cycles */

        /* Read the current bit to send (into carry)*/
        "l2: sbi 24, 0          \n\t" /* 2   cycles */
        "    lsl  %2            \n\t" /* 1   cycles */
        "    brcs l3            \n\t" /* 1/2 cycles */

        /* Send a 0 */
        "    cbi 24, 0          \n\t" /* 2   cycles */
        "    rjmp l4            \n\t" /* 2   cycles */

        /* Send a 1 */
        "l3: nop                \n\t" /* 1   cycles */
        "    cbi 24, 0          \n\t" /* 2   cycles */

        /* Next bit */
        "l4: dec %1             \n\t" /* 1   cycles */
        "    brne l2            \n\t" /* 1/2 cycles */

        /* Check if we're done */
        "l5: dec %0             \n\t" /* 1   cycles */
        "    brne l1            \n\t" /* 1/2 cycles */
    : "+a"(a), "+a"(b), "+a"(c)
    : [LEDS] "e" (LEDS)
    :
    );
}

int main() {
    /* D0 and D1 outputs, D2 input */
    DDRB  = (1<<0) | (1<<1) | (0<<2);
    PORTB = 0;

    /* Generate some kind of test pattern */
    LEDS[0].R = 127;
    LEDS[0].G = 0;
    LEDS[0].B = 0;

    LEDS[1].R = 0;
    LEDS[1].G = 127;
    LEDS[1].B = 0;

    LEDS[2].R = 0;
    LEDS[2].G = 0;
    LEDS[2].B = 127;

    while(1) {
        draw_leds();
    }
}
