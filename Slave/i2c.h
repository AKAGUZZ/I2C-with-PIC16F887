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

// PROTOTIPOS
void I2CSlave_init(unsigned char address);
unsigned char I2CSlave_RW0(void);
unsigned char I2CSlave_ReadOrWrite(void);
#endif	/* XC_HEADER_TEMPLATE_H */
