/*
 * File:   tx_main.c
 * Author: josep
 *
 * Created on August 27, 2024, 3:03 PM
 */
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#include <xc.h>
#include "lcd.h"
#include "soft_spi_slave.h"

char payload[3];

void main(void) {
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_set_cursor(1,1);
    lcd_clear();
    lcd_string("SLAVE: ");
    
    unsigned char receivedData;
    unsigned char sendData;

    spi_init(); // Initialize SPI as slave

    while (1) {
        receivedData = spi_transfer(2); // Receive data from master
        sendData = receivedData; // Echo received data back to master
        spi_transfer('a');
        
        lcd_char(receivedData);
    }
}
