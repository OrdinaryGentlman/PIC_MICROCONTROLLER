#ifndef hardware_spi_slave_h
#define hardware_spi_slave_h

#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 8000000 

void spi_slave_init()
{
    
  // SPI set to master mode
  // SCK rate set to Fosc/4
  // Serial Port Enabled
  SSPCON1 = 0x24;
  
  // Input data sampled at middle of data output time 
  // Transmit occurs on transition from Idle to active clock state
  // SMP Bit = 0. As written in the datasheet if slave mode it must me 0
  SSPSTAT = 0;
  
  // Since i am using pic18 familly: ADCON1 REG is used to Set SS (RA5/AN4) To Be a Digital IO pin.
  // If you are using PIC16 familly set ANSELA = 0 instead of ADCON1
  ADCON1 = 0x0A;
  TRISA5 = 1; // Set n_SS to input pin. This pin is used to know if the slave is selected by the master. the slave is selected if n_ss pin is 0
  
  // Configure The IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 1; // SCK -> Intput

  // Enable Interrupts. If you want to use the interrupt otherwise unselect the 3 lines below
  SSPIE = 1;
  PEIE = 1;
  GIE = 1;
}
 
// this function returns what the master has send
// response is the response to the master in the next iteration
unsigned char spi_read(unsigned char response)
{
    if(BF){
        unsigned char payload = SSPBUF; // Read the buffer
        SSPBUF = response;
        SSPIF = 0; // Clear the Interrupt Flag Bit
        SSPOV = 0; // Clear the Overflow Indicator Bit
        BF = 0; // Clear the Buffer-Filled Indicator Bit
        SSPIF = 0;
        return payload;
    }
}
#endif
