/*
 * File:   rx_main.c
 * Author: josep
 *
 * Created on August 27, 2024, 3:03 PM
 */
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#include <xc.h>
#include "lcd.h"
#include "uart.h"

short payload[5];

void main(void) {
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_set_cursor(1,1);
    lcd_clear();
    lcd_string("RX: ");
    
    //GIE = PEIE = 1;
    //uart_init(ASYNC_MODE | RX_MODE | RX_INT_EN, 9600); // asynchronous mode, rx mode only, interrupt mode
    
    uart_init(ASYNC_MODE, 9600); // asynchronous mode, rx mode only, polling mode
    //uart_init(0, 9600); // asynchronous mode, rx mode only, polling mode
    
    while (1) {
        uart_read_buffer(payload, 5); 
        uart_write('a');
        for(int i=0; i<5; i++)
            test((char) payload[i]);
    }
}

// if interrupt mode is used
void __interrupt() low_isr(void){
//    if(RCIF){
//        uart_read_buffer(payload, 5);       
//        for(int i=0; i<5; i++)
//            test((char) payload[i]);
//    }
}
