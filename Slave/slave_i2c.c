/*
* Creada por: Ing. Abiezer Hernandez O.
* Modificado y adaptado por: Estudiante Mecatrónico Reyes Guzmán Jose Angel
* Fecha de creacion: 25/11/2019
* Autor Original: Electronica y Circuitos
*
* Comentario: Archivo que contiene la definición de funciones para la gestión y control de la comunicación i2c en un
* PIC16F887 en modo SLAVE.
*/

#include "i2c.h"

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
