/*
 * File:   rx_main.c
 * Author: josep
 *
 * Created on August 27, 2024, 3:03 PM
 */
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#include <xc.h>
#include "lcd.h"
#include "soft_uart.h"


void main(void) {
    TRISD = 0;
    LATD = 0;

    lcd_init();
    lcd_set_cursor(1, 1);
    lcd_clear();
    lcd_string("RX: ");

    uart_init();

    while (1) {
        unsigned char x = uart_read(); // Receive data
        __delay_us(100);
        uart_write('a'); // Transmit 'a'
        test(x);
    }
}
