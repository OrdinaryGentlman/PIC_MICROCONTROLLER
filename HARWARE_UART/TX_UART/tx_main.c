/*
 * File:   tx_main.c
 * Author: josep
 *
 * Created on August 27, 2024, 3:03 PM
 */
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

#include <xc.h>
#include "lcd.h"
#include "uart.h"

short payload[5] = {'a', 'b', 'c', 'd', '\0'};

void main(void) {
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_set_cursor(1,1);
    lcd_clear();
    lcd_string("TX: ");

    uart_init(ASYNC_MODE, 9600); // TX MODE POLLING MODE
    //uart_init(0, 9600); // TX MODE POLLING MODE
    __delay_ms(100); // give the receiver time to intiate
      
    while (1) {
        uart_write_buffer(payload, 5);
        char payload = uart_read();
        test(payload);
        __delay_ms(2000);
    }
}

// If UART interrupt is enabled
void __interrupt(low_priority) low_isr(void){
}
