/* 
 * File:    master_i2c.h
 * Author:  Jose Angel Reyes Guzmán
 * Comments: Archivo que contiene los prototipos de las funciones para el control y gestion del protocolo de comunicación 
 *           I2C del microcontrolador PIC16F887 en modo Maestro.
 * Revision history: 1.0
 */
#include <xc.h>
#define _XTAL_FREQ 8000000
  
#ifndef MASTER_I2C_H
#define	MASTER_I2C_H

// PROTOTIPOS
void I2CMaster_init(void);
void I2CMaster_Start(void);
void I2CMaster_Address(unsigned char dato);
void I2CMaster_Write(unsigned char dato);
void I2CMaster_Stop(void);
unsigned char I2CMaster_Read(void);
void I2CMaster_Restart(void);
void I2CMaster_NACK(void);
void I2CMaster_ACK(void);        
#endif
/* XC_HEADER_TEMPLATE_H */
