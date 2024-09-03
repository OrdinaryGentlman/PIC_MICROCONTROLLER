/*
 * File:   tx_main.c
 * Author: josep
 *
 * Created on August 27, 2024, 3:03 PM
 */
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#include <xc.h>
#include "lcd.h"
#include "soft_i2c_master.h"

char payload[3];

void main(void) {
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_set_cursor(1,1);
    lcd_clear();
    lcd_string("MASTER: ");
    
    i2c_init();
      
    while(1){
        i2c_start();
        int nack = i2c_write(SLAVE_ADDR);
        if(!nack){
            nack = i2c_write(0x02);
            if(!nack){
                i2c_restart();
                nack = i2c_write(SLAVE_ADDR | 1);
                if(!nack){
                    payload[0] = i2c_read(0);
                    payload[1] = i2c_read(0);
                    payload[2] = i2c_read(1);
                }
            }
        }
        
        i2c_stop();
        
        lcd_string(payload);
        __delay_ms(2000);
    }
}
