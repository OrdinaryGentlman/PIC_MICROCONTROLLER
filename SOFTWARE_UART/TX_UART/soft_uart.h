/* 
 * File:   soft_uart.h
 * Author: josep
 *
 * Created on September 9, 2024, 11:19 AM
 */
#include <xc.h>
#include "lcd.h"

#ifndef SOFT_UART_H
#define	SOFT_UART_H

#define BAUD_RATE 9600  // Define baud rate
#define _XTAL_FREQ 8000000  // Define your microcontroller's clock frequency

#define TX_PIN  LATBbits.LATB0    // TX pin
#define TX_TRIS TRISBbits.TRISB0  // TX direction control
#define RX_PIN  PORTBbits.RB1     // RX pin
#define RX_TRIS TRISBbits.TRISB1  // RX direction control

// Delay per bit for the chosen baud rate
#define BIT_DELAY_US (10000000 / BAUD_RATE)

// Initialize the TX pin
void uart_init() {
    TX_TRIS = 0;  // Set TX pin as output
    TX_PIN = 1;   // Idle state (High)
    RX_TRIS = 1;  // Set RX pin as input
}

// Transmit a single byte using software UART
void uart_write(unsigned char data) {
    unsigned char i;

    // Start bit (low)
    TX_PIN = 0;
    __delay_us(BIT_DELAY_US);

    // Send 8 data bits, LSB first
    for (i = 0; i < 8; i++) {
        TX_PIN = (data >> i) & 0x01;  // Transmit each bit
        __delay_us(BIT_DELAY_US);     // Wait for the bit duration
    }

    // Stop bit (high)
    TX_PIN = 1;
    __delay_us(BIT_DELAY_US);
}

// Receive a single byte using software UART
unsigned char uart_read() {
    unsigned char data = 0;
    unsigned char i;

    // Wait for the start bit (low)
    while (RX_PIN);

    __delay_us(BIT_DELAY_US / 2);  // Half-bit delay for synchronization

    // Read 8 data bits, LSB first
    for (i = 0; i < 8; i++) {
        __delay_us(BIT_DELAY_US);  // Wait for the bit duration
        if (RX_PIN) {
            data |= (1 << i);  // Read each bit
        }
    }

    // Wait for the stop bit (high)
    __delay_us(BIT_DELAY_US);

    return data;
}

#endif	/* SOFT_UART_H */

