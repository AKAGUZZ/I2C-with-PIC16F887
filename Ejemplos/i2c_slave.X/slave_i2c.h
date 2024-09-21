/* 
 * File: slave_i2c.h  
 * Author: Jose Angel Reyes Guzmán
 * Comments: Archivo que gestiona el protocolo de comunicación I2C de un
 *           dispositivo es el maestro.
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SLAVE_I2C_H
#define	SLAVE_I2C_H

// PROTOTIPOS
void I2CSlave_init(unsigned char address);
unsigned char I2CSlave_Write(void);
void I2CSlave_Read(unsigned char dato);

// DEFINICIÓN DE MODULOS
void I2CSlave_init(unsigned char address){
    // In Slave mode, the SCL and SDA pins must be configured as inputs 
    // (TRISC<4:3> set))
    TRISCbits.TRISC3 = 1;   // RC3 as input - SCL
    TRISCbits.TRISC4 = 1;   // RC4 as input - SCL
    
    SSPSTAT = 0x80; // Checar
    SSPADD = address;       // Dirección
    SSPCON = 0x36;       // 0011 0110
    /* 1<<5: Enables the serial port and configures the SDA and SCL pins as the source of the serial port pins
     * 1<<4:  In I2C Slave mode: SCK release control - Enable clock 
     * 0110: I2C Slave mode, 7-bit address 
    */
    //SSPCONbits.CKP = 1;
    //SSPCONbits.CKP = 0;  // Quiza esto no es dejar CKP en 1
    //SSPCON2 = 0x01;
}

unsigned char I2CSlave_Write(void){
    unsigned char dato0;
    uint8_t regAdd = 1;
    if(SSPSTATbits.D_nA == 0){              // Last byte was an address
        uint8_t temp = 0;
        regAdd = 1;                     // Next byte register address
        temp = SSPBUF;                  // Clear BF
        SSPCONbits.CKP = 1;             // Release clock stretch
    }
        
    if(SSPSTATbits.D_nA == 1){
        dato0 = SSPBUF;
    }
    SSPCONbits.CKP = 1;
    return dato0;
}

void I2CSlave_Read(unsigned char dato){
    SSPBUF = dato;
    SSPCONbits.CKP = 1;
    while(BF);
}


#endif	/* XC_HEADER_TEMPLATE_H */

