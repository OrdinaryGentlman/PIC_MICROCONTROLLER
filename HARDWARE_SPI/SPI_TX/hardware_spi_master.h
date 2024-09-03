#ifndef hardware_spi_master_h
#define hardware_spi_master_h

#define _XTAL_FREQ 8000000

unsigned char payload = '\0';

void spi_master_init()
{
  // SPI set to master mode
  // SCK rate set to Fosc/4
  // Serial Port Enabled
  SSPCON1 = 0x20;
   
  // Input data sampled at middle of data output time
  // Transmit occurs on transition from Idle to active clock state
  SSPSTAT = 0;
  
  // Since i am using pic18 familly: ADCON1 REG is used to Set SS (RA5/AN4) To Be a Digital IO pin.
  // If you are using PIC16 familly set ANSELA = 0 instead of ADCON1
  ADCON1 = 0x0A;
  TRISA5 = 0; // Set n_SS to output pin. This pin is used to select the slave.
  
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 0; // SCK -> Output
  
  // If Interrupts Are Needed, Un-comment The Line Below
  // SSPIE = 1; PEIE = 1; GIE = 1;
}
 
// this function write a byte to the slave, and get the slave response to what the master has sent in the last iteration. 
unsigned char spi_write(unsigned char Data)
{
  SSPBUF = Data; // Transfer The Data To The Buffer Register
   while(!BF); // Un-comment it if you're Not Using Interrupts!
   return SSPBUF;
  // The Above While Loop Protects Against Writing To The Buffer Before
  // The Previous Transmission Ends
}
#endif
