/*
 * File:   i2c_slave.c
 * Author: Jose Angel Reyes Guzman
 * Comentario:
 * Created on 16 de marzo de 2024, 10:35 PM
 */


#include <pic16f887.h>
#include "config.h"
#include "slave_i2c.h"

unsigned char dato_rx;
uint8_t temp1 = 0;
//uint8_t regAdd = 1;
short z;
void main(void) {
    salidas();
    entradas();
    I2CSlave_init(0xA0);
    i2c_IntSetUpS();
    
    while(1){
        dato_rx = PORTA & 0x0F;
        PORTD = dato_rx;
    }
    return;
}

void __interrupt() INT_I2C()
{
    if(PIR1bits.SSPIF == 1) 
    {
        if(i2c_errorRead() != 0){
            i2c_errorData();
        }
        
        else{
            if (!SSPSTATbits.R_nW){ // Escribir en el esclavo
                dato_rx = I2CSlave_Write();
            }
            
            if (SSPSTATbits.R_nW){ // Leer el esclavo
                I2CSlave_Read(dato_rx);
            }
        }
        
        if(PIR2bits.BCLIF == 1){
            PORTD = (1<<3);
            temp1 = SSPBUF;                            
            PIR2bits.BCLIF = 0;                       
            SSPCONbits.CKP = 1;                      
        }
        PIR1bits.SSPIF = 0;
    }
}