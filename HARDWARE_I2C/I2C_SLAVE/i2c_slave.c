#include <xc.h>
#include <string.h>
#include <pic18f458.h>
#include "i2c_slave.h"

// Configuration settings
#pragma config WDT = OFF    // Watchdog timer disabled

void main(void) {
    i2c_init(SLAVE_ADDR);

    GIE = 1;
    PEIE = 1;              
    
    // PORTD ARE USED FOR THE LCD
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_clear();
    lcd_set_cursor(1,1);
    lcd_string("Slave: ");
	
	//initialize data to be send to the master, once the master request reading for slave
	tx_payload[0] = 0x68; // 'h'
	tx_payload[1] = 0x69; // 'i'
	tx_payload[2] = 0x70; // 'p'
    tx_payload[3] = '\0';
    
    while(1);
}


void __interrupt(low_priority)  low_isr(void){
	GIE = 0;
	PEIE = 0;

	// if address match SSPIF will be set for this slave
	if(SSPIF) i2c_tx_rx(lcd_char); // name of the function is passed as a parameter

    PEIE = 1;
    GIE = 1;
}
