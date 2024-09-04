/*
 * File:   tx_main.c
 * Author: josep
 *
 * Created on August 27, 2024, 3:03 PM
 */
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#include <xc.h>
#include "lcd.h"
#include "soft_spi_master.h"

char payload[3];

void main(void) {
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_set_cursor(1,1);
    lcd_clear();
    lcd_string("MASTER: ");
    
    spi_init();
      
    unsigned char receivedData;

    spi_init(); // Initialize SPI as master

    while (1) {
        spi_transfer('a'); // Send data to slave
        receivedData = spi_transfer(2); 
        lcd_char(receivedData);
        __delay_ms(10); // Small delay
    }
}
