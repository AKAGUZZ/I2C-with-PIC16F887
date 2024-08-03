/* #include "i2c.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Modificado y adaptado por: Estudiante Mecatrónico Reyes Guzmán Jose Angel
* Fecha de creacion: 25/11/2019
* Electronica y Circuitos
*
* Comentario: Archivo que contiene los prototipos de funciones para la gestión y control de la comunicación i2c en un
* PIC16F887 en modo SLAVE.
*/

#include <xc.h>
#define _XTAL_FREQ 8000000

#ifndef SLAVE_I2C_H
#define	SLAVE_I2C_H

// Prototipos de Módulos
void entradas(void);
void salidas(void);
void i2c_setupS(void);     
void i2c_IntSetUpS(void);   
short i2c_errorRead(void);
short i2c_writeMode(void);
short i2c_readMode(void);
void i2c_errorData(void);
unsigned char i2c_readSlave(void);
void i2c_writeSlave(char dato_i2c);
#endif	/* XC_HEADER_TEMPLATE_H */
