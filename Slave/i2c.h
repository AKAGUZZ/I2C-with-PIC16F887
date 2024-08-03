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

#define I2C_SLAVE_MODE

#ifdef I2C_SLAVE_MODE
void I2C_Init_Slave(unsigned char add_slave);
unsigned char I2C_Read_Slave(void);
void I2C_Write_Slave(char dato_i2c);
void I2C_Error_Data(void);
short I2C_Write_Mode(void);
short I2C_Read_Mode(void);
short I2C_Error_Read(void);
#endif
