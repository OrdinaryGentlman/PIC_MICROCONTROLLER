/* 
 * File:   soft_spi_master.h
 * Author: josep
 *
 * Created on August 27, 2024, 3:02 PM
 */

#ifndef SOFT_SPI_MASTER_H
#define	SOFT_SPI_MASTER_H

#define _XTAL_FREQ  8000000

#define SPI_SCK  LATCbits.LATC3  // Clock pin
#define SPI_MOSI LATCbits.LATC4 // Master Out Slave In pin
#define SPI_MISO PORTCbits.RC5  // Master In Slave Out pin
#define SPI_CS   LATCbits.LATC2 // Chip Select pin

void spi_init(void) {
    TRISCbits.TRISC3 = 0; // SCK as output
    TRISCbits.TRISC4 = 0; // MOSI as output
    TRISCbits.TRISC5 = 1; // MISO as input
    TRISCbits.TRISC2 = 0; // CS as output

    SPI_CS = 1;  // Deselect slave
    SPI_SCK = 0; // Initial clock state
}

unsigned char spi_transfer(unsigned char data) {
    unsigned char receivedData = 0;
    SPI_CS = 0; // Select slave
    for (char i = 0; i < 8; i++) {
        SPI_MOSI = (data & 0x80) ? 1 : 0; // Send MSB first
        data <<= 1;       
        __delay_us(20);       
        SPI_SCK = 1; // Clock high
        __delay_us(20); // Small delay
        receivedData <<= 1;
        if (SPI_MISO) receivedData |= 0x01;
        SPI_SCK = 0; // Clock low
        __delay_us(20); // Small delay
    }

    SPI_CS = 1; // Deselect slave
    return receivedData;
}
#endif

