/* 
 * File:   soft_spi_slave.h
 * Author: josep
 *
 * Created on August 27, 2024, 3:02 PM
 */

#ifndef SOFT_SPI_SLAVE_H
#define	SOFT_SPI_SLAVE_H

#define _XTAL_FREQ  8000000

#define SPI_SCK PORTCbits.RC3   // Clock pin
#define SPI_MOSI PORTCbits.RC4  // Master Out Slave In pin
#define SPI_MISO LATCbits.LATC5 // Master In Slave Out pin
#define SPI_CS   PORTCbits.RC2  // Chip Select pin

void spi_init(void) {
    TRISCbits.TRISC3 = 1; // SCK as input
    TRISCbits.TRISC4 = 1; // MOSI as input
    TRISCbits.TRISC5 = 0; // MISO as output
    TRISCbits.TRISC3 = 1; // CS as input

    SPI_MISO = 0; // Initial output state
}

unsigned char spi_transfer(unsigned char data) {
    unsigned char receivedData = 0;

    while (SPI_CS == 1); // Wait until selected by master

    for (char i = 0; i < 8; i++) {
        SPI_MISO = data & 0x80 ? 1 : 0; // Send MSB first
        data <<= 1;
        while (SPI_SCK == 0); // Wait for clock low
        __delay_us(5);
        receivedData <<= 1;
        if (SPI_MOSI) receivedData |= 0x01;
        while (SPI_SCK == 1); // Wait for clock high
        __delay_us(5);
    }

    return receivedData;
}

#endif

