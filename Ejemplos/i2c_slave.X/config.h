/* 
 * File:        config.h
 * Author:      si
 * Comments:    Archivo de cabecerra de configuración del PIC16F887
 * Revision history: 0.1
 */

#ifndef CONFIG_H
#define	CONFIG_H

// Palabras de configuración de bits en el PIC16F887
// CONFIG1
#pragma config FOSC = XT       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h> // Librería que implementa el lenguaje XC8

// Frecuencia de oscilación del cristal
#define _XTAL_FREQ 4000000

// Prototipos de Módulos
void entradas(void);
void salidas(void);

void i2c_IntSetUpS(void);   //LISTO
short i2c_errorRead(void);
short i2c_writeMode(void);
short i2c_readMode(void);
void i2c_errorData(void);
unsigned char i2c_readSlave(void);
void i2c_writeSlave(char dato_i2c);

//Definicion de Módulos
void entradas(void){
    ANSEL &= ~(15<<0);
    TRISA |= (15<<0);
}

void salidas(void)
{
    TRISD &= ~(15<<0);
    PORTD &= ~(15<<0);
}

// ============================= I2C CONFIGURATION =============================
// Inicializar I2C
// Otras funciones del i2c
void i2c_IntSetUpS(void){
    INTCON |= (1 << 7); //Habilitar interrupciones globales
    
    //Configuración de interrupciones en el periferico MSSP
    INTCON |= (1 << 6); // Habilitamos interrupciones en perifericos
    PIE1bits.SSPIE = 1; // Enables the MSSP interrupt
    PIE2bits.BCLIE = 1; // Enables Bus Collision interrupt
    
    PIR1bits.SSPIF = 0; // Limpiar la bandera de interrupción
    PIR2bits.BCLIF = 0; // No bus collision has occurred
}

short i2c_errorRead(void){
    SSPCONbits.CKP = 0; // Holds clock low (clock stretch). (Used to ensure data setup time.)
    return (SSPCONbits.SSPOV || (SSPCONbits.WCOL)) ? 1 : 0;
}

short i2c_writeMode(void){
    return(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) ? 1 : 0;
}

short i2c_readMode(void){
    return(!SSPSTATbits.D_nA && SSPSTATbits.R_nW) ? 1 : 0;
}

void i2c_errorData(void){
    short z;
    SSPCONbits.CKP = 0;
    z = SSPBUF;
    SSPCONbits.SSPOV = 0; // Set No Overflow
    SSPCONbits.WCOL =  0; // Set No Colition
    SSPCONbits.CKP = 1;   // Enable clock
    SSPCONbits.SSPM3 = 0;
}

unsigned char i2c_readSlave(void){
    short z;
    unsigned char dato_i2c;
    z = SSPBUF;
    while(!BF)
    dato_i2c = SSPBUF;
    SSPCONbits.CKP = 1;
    SSPCONbits.SSPM3 = 0;
    return dato_i2c;
}

void i2c_writeSlave(char dato_i2c){
    short z;
    z = SSPBUF;
    BF = 0;
    SSPBUF = dato_i2c; // Cargar al buffer el dato
    SSPCONbits.CKP = 1;
    while (SSPSTATbits.BF == 1); //Data transmit in progress (does not include the ACK and Stop bits), SSPBUF is full
    // Mientras se transmita informacion no hacer nada
}

#endif	/* XC_HEADER_TEMPLATE_H */

