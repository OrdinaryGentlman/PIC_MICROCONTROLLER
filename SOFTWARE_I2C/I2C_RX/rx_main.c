/*
 * File:   tx_main.c
 * Author: josep
 *
 * Created on August 27, 2024, 3:03 PM
 */
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#include <xc.h>
#include "lcd.h"
#include "soft_i2c_slave.h"

void main(void) {
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_set_cursor(1,1);
    lcd_clear();
    lcd_string("SLAVE: ");
    
    i2c_init();

    while(1){
        i2c_wait_for_start();
        char payload = i2c_read(0);
        if(payload == SLAVE_ADDR){
            payload = i2c_read(0);
            if(payload == 0x02){
                i2c_wait_for_restart();
                payload = i2c_read(0);
                if(payload == (SLAVE_ADDR | 1)){
                    char nack = i2c_write('A');
                    if(!nack){
                        nack = i2c_write('R');
                        if(!nack){
                            nack = i2c_write('O');
                        }
                    }
                }
            }
        }
        i2c_wait_for_stop(); 
    }
}