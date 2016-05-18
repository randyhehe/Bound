#ifndef SRHELPERS_H
#define SRHELPERS_H

#include "ledmatrix.h"

#define PORT PORTB
#define DATAPIN 0
#define SHIFTPIN 1
#define LATCHPIN 2

#define COLUMNPORT PORTD

// Shift one value
void shiftSR(unsigned char i) {
    PORT = i << DATAPIN;
    PORT = PORT | (0x01 << SHIFTPIN);
    PORT = 0x00;
}

// Latch current contents
void latchSR() {
    PORT = 0x01 << LATCHPIN;
}

// Clear shift register
void clearSR() {
    for (int i = 0; i < 8; i++) {
        shiftSR(1);
    } latchSR();
}

#endif
