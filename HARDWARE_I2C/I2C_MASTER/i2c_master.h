#ifndef I2C_MASTER_H
#define	I2C_MASTER_H

#include <xc.h>
#include "lcd.h"

#define _XTAL_FREQ  4000000

#define TX_LENGTH	32     // bytes
#define RX_LENGTH	32     // bytes

void i2c_init(const unsigned long baudrate)
{
  SSPCON1 = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*baudrate))-1;
  SSPSTAT = 0;
  TRISC3 = 1;        //Setting as input as given in datasheet
  TRISC4 = 1;        //Setting as input as given in datasheet
}

void i2c_wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void i2c_start()
{
  SEN = 1;
  while(SEN); // wait for start to complete
  while(!SSPIF); // after start is completed, SSPIF became 1 we have to clear it
  SSPIF = 0;
}

void i2c_restart()
{
  i2c_wait();
  RSEN = 1;
  while (RSEN); // wait for restart to complete
}

void i2c_stop()
{
  PEN = 1;
  while(PEN); // wait for stop to complete
}

void i2c_ack(){
    ACKDT = 0;
    ACKEN = 1;
    while(ACKEN); // wait for ACK to finish
}

void i2c_nack(){
    ACKDT = 1;
    ACKEN = 1;
    while(ACKEN); // wait for NACK to finish
}

void i2c_write(unsigned d)
{
  SSPBUF = d;
  while((SSPSTAT & 0x04) || (SSPSTAT & 0x01));
}

unsigned char i2c_read(unsigned char n_ack)
{
  unsigned char temp;
  i2c_wait();
  RCEN = 1;
  i2c_wait();
  temp = SSPBUF; 
  ACKDT = (n_ack)?1:0;
  ACKEN = 1;
  return temp;
}
#endif	/* I2C_MASTER_H */

