#include <xc.h>
#include "i2c_master.h"

// Configuration settings
#pragma config WDT = OFF    // Watchdog timer disabled

#define SLAVE_ADDR 0x88

unsigned char tx_payload[TX_LENGTH];
unsigned char rx_payload[TX_LENGTH];

void main(void) {
    i2c_init(100000);
    
    // PORTD ARE USED FOR THE LCD
    TRISD = 0;
    LATD = 0;
    
    lcd_init();
    lcd_clear();
    lcd_set_cursor(1,1);
    lcd_string("Master: ");
    
    __delay_ms(1000);
    
    while(1){
        // the master wants to send data to the slave
        i2c_start();         // Start condition
        i2c_write(SLAVE_ADDR);     // 7 bit address + Write
        i2c_write('x');      // Write data
        i2c_stop();          //Stop condition
        
        __delay_ms(2000);
        
        i2c_start();         // Start condition
        i2c_write(SLAVE_ADDR);     // 7 bit address + Write
        i2c_write('y');      // Write data
        i2c_stop();          // Stop condition
        
        __delay_ms(2000);
        
        i2c_start();         // Start condition
        i2c_write(SLAVE_ADDR);     // 7 bit address + Write
        i2c_write('z');      // Write data
        i2c_stop();          // Stop condition
        
        __delay_ms(2000);
        
        
        // The master wants to read data from the slave
        i2c_start();                    // Start condition
        i2c_write(SLAVE_ADDR | 0x01);   // 7 bit address + read
        rx_payload[0] = i2c_read(0);    // send ack
        rx_payload[1] = i2c_read(0);    // send ack
        rx_payload[2] = i2c_read(1);    // send nack
        rx_payload[3] = '\0';
        i2c_stop();                     //Stop condition
        
        lcd_string(rx_payload); // display the data sent from the slave to the master on the master's LCD screen
        
        __delay_ms(2000);
    }
}
