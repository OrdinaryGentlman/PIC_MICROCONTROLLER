#ifndef I2C_SLAVE_H
#define	I2C_SLAVE_H

#include <stddef.h> 
#include "lcd.h"

#define SLAVE_ADDR  0x88

#define TX_LENGTH	32     // bytes
#define RX_LENGTH	32     // bytes

char tx_payload[TX_LENGTH];
char rx_payload[RX_LENGTH];

void i2c_init(unsigned char address)
{
	SSPSTAT = 0x80;     // 100 khz
	SSPADD = address;   // slave address
	SSPCON1 = 0x36;     // SSPEN Enabled, and CKP is set to 1 (clock released), 7 bits address slave
	SSPCON2 = 0x01;     // clock stretching is enabled
	TRISC3 = 1;   
	TRISC4 = 1;   
	SSPIE = 1; // enable i2c interrupt
	SSPIF = 0;
}

void i2c_reset(){
	SSPIF = 0;
	SSPEN = 0;
	while(SSPEN); 
	i2c_init(SLAVE_ADDR);
}

// IF THE MASTER IS SENDING DATA TO THE SLAVE => THE DATA READ BY THE SLAVE ARE IN THE rx_payload ARRAY
// IF THE SLAVE IS SENDING DATA TO THE MASTER => DATA TO BE SEND FOR THE MASTER MUST BE PLACED IN tx_payload ARRAY
// The void *on_byte_received(unsigned char) is an optional DELEGATE, if you want to perform a function after reading each byte from the master, you can pass your function name as parameter, if not just pass NULL
void i2c_tx_rx(void (*on_byte_received)(unsigned char)) {                     
    SSPIF = 0;
    CKP = 0; // Pull clock low, once the address is received, to prepare for receiving or transmitting data

    if ((SSPCON1bits.SSPOV) || (SSPCON1bits.WCOL)) {
        char z = SSPBUF; // Read the previous value to clear the buffer
        SSPCON1bits.SSPOV = 0; // Clear the overflow flag
        SSPCON1bits.WCOL = 0;  // Clear the collision bit
        SSPCON1bits.CKP = 1;
        SSPIF = 0;
    }

    unsigned char addr = SSPBUF; // Get the address 

    // If R_nW bit of SSPSTAT is set to 1 =>  the master wants to read data from the slave
    // Here the data from the slave to the master are sent from tx_payload
    if(SSPSTATbits.R_nW){
        for(int x = 0; x < TX_LENGTH ; x++){
            CKP = 0; // Stretch the clock (Clock is set to 0) to prepare the data to send to the master
            SSPBUF = tx_payload[x];
            CKP = 1; // Data is ready to be sent, we release the clock for the master
            while(!SSPIF && !SSPSTATbits.P); // Wait for transmission to be completed
            SSPIF = 0;
            if(SSPSTATbits.P) break; // If the master sends a NACK => stop transmission
        }
    } else { 
        // R_nW bit is 0 => master wants to send data to the slave
        // Here the data sent from the master to the slave are in the rx_payload array
        for(char i = 0; i < RX_LENGTH; i++){
            while(!SSPIF && !SSPSTATbits.P); // SSPIF is set if a byte is received. Stop bit flag is set if no bytes anymore is received from the master.
            SSPIF = 0; // We clear the flag once the byte is received
            if(SSPSTATbits.P) break; // If a stop bit received stop reception. The master has finished sending data
            CKP = 0; // Stretch the clock, if the slave wants to perform a function with the received byte
            
            rx_payload[i] = SSPBUF; 
            
            // If a callback is provided, call it with the received byte
            if (on_byte_received != NULL) {
                on_byte_received(rx_payload[i]);
            }

            // When the read is done release the clock => CKP is set to 1
            CKP = 1; 
        }
    }

    CKP = 1;
    i2c_reset(); // Reset the i2c hardware to clear start and stop bits
}

#endif	/* I2C_MASTER_H */

