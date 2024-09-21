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
#pragma config FOSC = HS       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
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
#define _XTAL_FREQ 8000000

// Prototipos de Módulos
void salidas(void);
void entradas(void);
//Definicion de Módulos
void salidas(void){
    TRISD = 0x00;
    PORTD = 0x00;
}

void entradas(void){
    ANSELH = 0x00; //Todo PORTB configurado digitalmente
    TRISB |= (1<<0); //RB0 como entrada
    ANSEL = 0x00;
    TRISA |= (7<<0);
}

#endif	/* XC_HEADER_TEMPLATE_H */

