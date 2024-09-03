/*
 * File:   soft_i2c_slave.h
 * Author: josep
 *
 * Created on August 27, 2024, 3:02 PM
 */
#ifndef SOFT_I2C_SLAVE_H
#define	SOFT_I2C_SLAVE_H

#define _XTAL_FREQ  8000000
#define SLAVE_ADDR  0x88

#define SCL         TRISB4      // I2C bus
#define SDA         TRISB1      //
#define SCL_IN      PORTBbits.RB4  //
#define SDA_IN      PORTBbits.RB1  //

void i2c_init(void) {
    SDA = SCL = 1;  // Set SDA and SCL as input to release the bus
    LATBbits.LB4 = LATBbits.LB1 = 0;
}

void dly(void) {
    __delay_us(5);  // Short delay to emulate I2C timing
}

void i2c_wait_for_start(void) {
    while (SCL_IN && SDA_IN);   // Wait for start condition (SDA goes low while SCL is high)
}

void i2c_wait_for_restart(void) {
    // Wait for SCL to be high and SDA to go low (restart condition)
    while (!(SCL_IN == 1 && SDA_IN == 1)); // Wait until the bus is not idle
    while (SCL_IN == 1 && SDA_IN == 1); // Wait until the bus is not idle
    while (!(SCL_IN == 1 && SDA_IN == 0)); // Detect restart (SDA goes low while SCL is high)
    while (SCL_IN == 1 && SDA_IN == 0); // Detect restart (SDA goes low while SCL is high)
}

void i2c_wait_for_stop(void) {
    while (!(SDA_IN && SCL_IN)); // Wait for stop condition (SDA goes high while SCL is high)
    SCL = SDA = 1;
}

unsigned char i2c_write(unsigned char d) {   
    SCL = 0;
    __delay_us(10);
    if(d&0x80) SDA = 1;
    else SDA = 0;
    d <<= 1;
    __delay_us(5);
    SCL = 1;
        
    for (char i = 1; i < 8; i++) {
        while(SCL_IN);
        __delay_us(5);
        if(d&0x80) SDA = 1;
        else SDA = 0;
        d <<= 1;
        while(!SCL_IN);
        __delay_us(5);
    }
    
    SDA = 1;
    while(!SCL_IN);
    __delay_us(5);
    
    char b = SDA_IN;
    
    while(SCL_IN);
    __delay_us(5);
    
    if(b) SCL = 1; // if NACK received release the clock
    else SCL = 0;
    
    return b;
}

unsigned char i2c_read(unsigned char nack) {
    char data = 0;
    for (char x = 0; x<8; x++) {
        data <<= 1;
        while (!SCL_IN);  // Wait for SCL to go high
        __delay_us(5);
        if (SDA_IN) data |= 1;      
        while(SCL_IN);
    }
    
    SCL = 0; // The slave stretch the clock low
    
    if(nack) SDA = 1; // Send a NACK end of reading
    else SDA = 0; // send an ACK
    /*
     if you want the slave to perform an action directly after the read value from the slave,
     * do it here above the dly() function
     */
    dly();
    SCL = 1; // The slave releases the clock
    while(SCL_IN);
    __delay_us(5);
    SDA = 1;
    return data;
}
#endif	
