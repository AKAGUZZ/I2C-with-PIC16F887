/*
 * File:   master_i2c.c
 * Author: Reyes Guzmán Jose Angel
 * Comentario: Archico que contiene la definición de Módulos para la gestión y control de la comunicación i2c en modo 
 *             maestro.
 * Created on 9 de junio de 2024, 12:02 PM
 */


#include "master_i2c.h"

// DEFINICION DE MODULOS
void I2CMaster_init(void){
    TRISCbits.TRISC3 = 1; // RC3 - SCL como entrada
    TRISCbits.TRISC4 = 1; // RC4 - SDA como entrada
    
    SSPADD = (8000000 / ( 4 * (100000 + 1) ));  // Generador de tasa de baudios
    SSPCON = 0x28;
    // 0010 1000
    // SSPEN: Synchronous Serial Port Enable bit
    // 1 = Enables the serial port and configures the SDA and SCL pins as the source of the serial port pins
    // SSPM<3:0>: Synchronous Serial Port Mode Select bit
    // 1000 = I2C Master mode, clock = FOSC / (4 * (SSPADD+1))
}
void I2CMaster_Start(void){
    SSPCON2bits.SEN = 1;
    while(SEN);
    PIR1bits.SSPIF = 0;
}

void I2CMaster_Address(unsigned char dato){
    SSPBUF = dato;
    while(!SSPIF);
    PIR1bits.SSPIF = 0;
    if (SSPCON2bits.ACKSTAT){
        SSPCON2bits.PEN = 1;
        while(PEN);
        return;
    }
}

void I2CMaster_Write(unsigned char dato){
    SSPBUF = dato;
    while(!SSPIF);
    PIR1bits.SSPIF = 0;
}

void I2CMaster_Stop(void){
    SSPCON2bits.PEN = 1;
    while(PEN);
}

unsigned char I2CMaster_Read(void){
    unsigned char TempData;
    SSPCON2bits.RCEN = 1;     // Enables Receive mode for I2C. RCEN cleared automatically
    while (!SSPSTATbits.BF);  // Wait for the recieve to complete
    TempData = SSPBUF;   
}

void I2CMaster_Restart(void){
    SSPCON2bits.RSEN = 1; // Repeated Start Condition.
    // Initiate Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware.
    while(RSEN); //Wait for Restart condition to complete
    PIR1bits.SSPIF = 0; // Clear SSP interrupt flag
}

void I2CMaster_NACK(void){
    SSPCON2bits.ACKDT = 1; // Prepare to send NACK.  1 = Not Acknowledge
    SSPCON2bits.ACKEN = 1; // Iniciate NACK to Slave. Initiate Acknowledge 
    //sequence on SDA and SCL pins, and transmit ACKDT data bit.
    //Automatically cleared by hardware.
    while(ACKEN); //Wait for NACK to complete
}

void I2CMaster_ACK(void){
    SSPCON2bits.ACKDT = 0; // Acknowledge
    SSPCON2bits.ACKEN = 1; // Initiate Acknowledge sequence on SDA and SCL pins, and transmit ACKDT data bit.
    while(ACKEN); //Wait for NACK to complete
}
