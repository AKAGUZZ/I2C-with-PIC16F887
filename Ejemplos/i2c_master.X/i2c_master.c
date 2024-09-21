/*
 * File:   i2c_master.c
 * Author: guza_
 *
 * Created on 10 de marzo de 2024, 08:38 PM
 */

#include <pic16f887.h>
#include "config.h"
#include "master_i2c.h"

void main(void) {
    unsigned char dato_xd;
    salidas();
    entradas();
    I2CMaster_init();
    
    while(1){
        if (PORTAbits.RA0){
            PORTD ^= (1<<4);
            __delay_ms(100);
        }
        
        if (!PORTAbits.RA0){
            I2CMaster_Start();
            I2CMaster_Address(0xA0 + 1);
            dato_xd = I2CMaster_Read();
            I2CMaster_NACK();
            I2CMaster_Stop();
            PORTD = dato_xd;
            __delay_ms(10);
        }
        /*
        dato_xd = PORTA & 0x0F;
        PORTD = dato_xd;
        I2CMaster_Start();
        I2CMaster_Address(0xA0);
        I2CMaster_Write(dato_xd);
        I2CMaster_Stop();
        __delay_ms(50);
        */
    }
    
    return;
}