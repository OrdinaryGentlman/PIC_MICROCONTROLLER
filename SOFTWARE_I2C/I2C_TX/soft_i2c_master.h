/* 
 * File:   soft_i2c_master.h
 * Author: josep
 *
 * Created on August 27, 2024, 3:02 PM
 */

#ifndef SOFT_I2C_MASTER_H
#define	SOFT_I2C_MASTER_H

#define _XTAL_FREQ  8000000
#define SLAVE_ADDR  0x88

#define SCL     TRISB4 // I2C bus
#define SDA     TRISB1 //
#define SCL_IN  PORTBbits.RB4    //
#define SDA_IN  PORTBbits.RB1    //

void i2c_init(){
    SDA = SCL = 1;    
    LATBbits.LB4 = LATBbits.LB1 = 0;
}

void dly() {
    __delay_us(10);
}

void i2c_start(void)
{
  SDA = 1;             // i2c start bit sequence
  dly();
  SCL = 1;
  dly();
  SDA = 0;
  dly();
  SCL = 0;
  dly();
}

void i2c_restart(void) {
    SDA = 1;  // Make sure SDA is high
    dly();  
    SCL = 1;  // Make sure SCL is high
    dly();
    SDA = 0;  // Pull SDA low while SCL is high (Restart condition)
    dly();
    SCL = 0;  // Pull SCL low to start the clock for the next byte
    //dly();
}

void i2c_stop(void)
{
  SDA = 0;             // i2c stop bit sequence
  dly();
  SCL = 1;
  dly();
  SDA = 1;
  dly();
}

// master
unsigned char i2c_read(unsigned char nack) {
    unsigned char data = 0;

    dly();
    
    SDA = 1;
    SCL = 1;
    
    while(!SCL_IN);
    
    for (char x = 0; x<8; x++) {
        data <<= 1;
        if (SDA_IN) data |= 1;
        SCL = 1;
        dly();
        SCL = 0;
        dly();
    }
    
    if(nack) SDA = 1; // Send a NACK end of reading
    else SDA = 0; // send an ACK
    dly();
    SCL = 1;
    dly();
    
    SCL = 0;
    SDA = 0;
    
    return data;
}

unsigned char i2c_write(unsigned char d)
{
  unsigned char b;
  for(unsigned char x=8; x>0; x--) {
    if(d&0x80) SDA = 1;
    else SDA = 0;
    d <<= 1;   
    SCL = 1; 
    dly(); 
    SCL = 0;
    dly();
  }

  SDA = 1;
  SCL = 1;
  
  __delay_us(5); 
  
  while(!SCL_IN);
  b = SDA_IN;          // possible ACK bit
  SCL = 0;
  return b;
}
#endif

