/*
 * File:   tx_main.c
 * Author: Joseph Elaro
 *
 * Created on August 26, 2024, 9:26 PM
 */


#include <xc.h>
#include "hardware_spi_master.h"
#include "lcd.h"

// Configuration settings
#pragma config WDT = OFF    // Watchdog timer disabled

void main(void) {
    // PORTD ARE USED FOR THE LCD
    TRISD = 0;
    LATD = 0;

    lcd_init();
    lcd_clear();
    lcd_set_cursor(1,1);
    lcd_string("TX: ");
    
    __delay_ms(1000);
    
    spi_master_init();
    
    while(1){
        lcd_char(spi_write('h'));
        __delay_ms(1000);
    }
}