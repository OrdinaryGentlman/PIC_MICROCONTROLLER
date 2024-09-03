/*
 * File:   tx_main.c
 * Author: Joseph Elaro
 *
 * Created on August 26, 2024, 9:26 PM
 */

// Configuration settings
#pragma config WDT = OFF    // Watchdog timer disabled

#include <xc.h>
#include "hardware_spi_slave.h"
#include "lcd.h"

unsigned char payload = '\0';

void main(void) {
    // PORTD ARE USED FOR THE LCD
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_clear();
    lcd_set_cursor(1,1);
    lcd_string("RX: ");
    
    __delay_ms(1000);
        
    spi_slave_init();
   
    while(1){
        // if you don't want the use interrupt mode unselect this line
        //if(SSPIF) lcd_char(spi_read('a'));
    };
}

void __interrupt(low_priority)  low_isr(void){
	GIE = 0;
	PEIE = 0;

    if(SSPIF)
    {
        payload = spi_read('a');
        lcd_char(payload);
        SSPIF = 0; // Clear The Interrupt Flag
    }

    PEIE = 1;
    GIE = 1;
}
