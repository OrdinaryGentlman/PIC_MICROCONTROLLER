/* 
 * File:   uart_tx.h
 * Author: josep
 *
 * Created on September 4, 2024, 11:19 AM
 */
#include <xc.h>
#include "lcd.h"
#include <math.h>

#ifndef UART_TX_H
#define	UART_TX_H

#define _XTAL_FREQ  8000000    

#define ASYNC_MODE  0x01   // Asynchronous mode
#define DATA_INV    0x02   // TX data inverted
#define RX_INT_EN   0x04   // On receive UART interrupt enabled
#define NINE_BITS   0x08   // 9 bits UART

char uart_init(unsigned char config, unsigned long BAUDRATE) {
    // This section automatically set the value of SPBRG, SPBRGH, BRGH and BRG16
    // <editor-fold defaultstate="collapsed" desc="BAUDRATE Section">
    const double frequency = _XTAL_FREQ + 0.0;
    const double baudrate = BAUDRATE + 0.0;
    long x, y, z;
    double e1, e11, e2, e22, e3, e33;

    // Simulated baudrate input for example purposes
    x = (long) ((frequency / (64 * baudrate)) - 1);
    y = (long) ((frequency / (16 * baudrate)) - 1);
    z = (long) ((frequency / (4 * baudrate)) - 1);

    double min = 0;

    if (x > 0) {
        e1 = fabs((frequency / (64 * (x + 1)) - baudrate) / baudrate);
        e11 = fabs((frequency / (64 * (x + 1 + 1)) - baudrate) / baudrate);
        if (e11 < e1) {
            e1 = e11;
            x = x + 1;
        }

        e2 = fabs((frequency / (16 * (y + 1)) - baudrate) / baudrate);
        e22 = fabs((frequency / (16 * (y + 1 + 1)) - baudrate) / baudrate);
        if (e22 < e2) {
            e2 = e22;
            y = y + 1;
        }

        e3 = fabs((frequency / (4 * (z + 1)) - baudrate) / baudrate);
        e33 = fabs((frequency / (4 * (z + 1 + 1)) - baudrate) / baudrate);
        if (e33 < e3) {
            e3 = e33;
            z = z + 1;
        }

        min = fmin(e1, fmin(e2, e3));
    } else if (y > 0) {
        e2 = fabs((frequency / (16 * (y + 1)) - baudrate) / baudrate);
        e22 = fabs((frequency / (16 * (y + 1 + 1)) - baudrate) / baudrate);
        if (e22 < e2) e2 = e22;

        e3 = fabs((frequency / (4 * (z + 1)) - baudrate) / baudrate);
        e33 = fabs((frequency / (4 * (z + 1 + 1)) - baudrate) / baudrate);
        if (e33 < e3) e3 = e33;

        min = fmin(e2, e3);
    } else if (z > 0) {
        e3 = fabs((frequency / (4 * (z + 1)) - baudrate) / baudrate);
        e33 = fabs((frequency / (4 * (z + 1 + 1)) - baudrate) / baudrate);
        if (e33 < e3) e3 = e33;
        min = e3;
    } else {
        return -1;
    }

    if (min == e1) {
        BRGH = 0;
        BRG16 = 0;
        SPBRG = x;
    }// Low-speed mode 
    else if (min == e2) {
        BRGH = 1;
        BRG16 = 0;
        SPBRG = y;
        SPBRGH = y >> 8;
    }// High-speed mode
    else {
        BRGH = 1;
        BRG16 = 1;
        SPBRG = z;
        SPBRGH = z >> 8;
    }// High-speed mode with BRG16

    // </editor-fold> 

    // <editor-fold defaultstate="collapsed" desc="INIT - Section">    
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;

    // Inverted or non inverted data transmitted
    if ((config & DATA_INV) == 0x02) {
        TXCKP = 1;
        RXDTP = 1;
    } else {
        RXDTP = 0;
        TXCKP = 0;
    }

    // Interrupt enabled or disabled
    if ((config & RX_INT_EN) == 0x04) {
        //TXIE = 1;
        RCIE = 1;
    } else {
        RCIE = 0;
        TXIE = 0;
    }

    // 9 or 8 bits UART transmission 
    if ((config & NINE_BITS) == 0x08) {
        TX9 = 1;
        RX9 = 1;
    } else {
        TX9 = 0;
        RX9 = 0;
    }

    // Synchronous or Asynchronous uart mode
    if ((config & ASYNC_MODE) == 0x01) {
        SYNC = 0;
    } else {
        SYNC = 1;
    }

    CREN = 1;
    SREN = 1;
    TXEN = 1;
    SPEN = 1; // Enable serial port
    // </editor-fold> 

    return 1;
}

void uart_write(short payload) {
    if (TX9) TX9D = payload >> 8;
    TXREG = payload;
    while (!TXIF); // Wait until TXREG become empty => data transfered from TXREG to TSR
    while (!TRMT); // Wait until data are shifted from TSR reg => data transmitted
}

void uart_write_buffer(short *data, int length) {
    for (int i = 0; i < length; i++) {
        if (TX9) TX9D = data[i] >> 8;
        TXREG = data[i]; // Load the current byte into the transmit register and move to the next byte
        while (!TXIF);
    }

    while (!TRMT);
}

short uart_read() {
    unsigned char receivedData = 0;
    while (!RCIF);
    if (FERR) {
        receivedData = RCREG;
        return 0x01FF;
    }

    if (OERR) {
        receivedData = RCREG;
        RCEN = 0;
        RCEN = 1;
        return 0x01FF;
    }
    receivedData = RX9D ? RCREG | 0x0100 : RCREG;
    return receivedData;
}

short* uart_read_buffer(short* buffer, int length) {
    for (int i = 0; i < length; i++) {
        buffer[i] = uart_read();
    }
    return buffer;
}
#endif	/* UART_TX_H */

