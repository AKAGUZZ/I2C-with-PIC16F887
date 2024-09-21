/* 
 * File:    master_i2c.h
 * Author:  Jose Angel Reyes Guzmán
 * Comments: Archivo que gestiona el protocolo de comunicación I2C de un
 *           dispositivo Maestro.
 * Revision history: 1.0
 */
#include <xc.h>
#define _XTAL_FREQ 8000000
  
#ifndef DS1307_CONTROL_H
#define	DS1307_CONTROL_H

// PROTOTIPOS
void DS_SETHora(char set[]);
void DS_SET_Date(char set_fecha[]);
void DS_ReadHora(uint8_t *segundos, uint8_t *minutos, uint8_t *horas);
void DS_ReadDate(uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);
void DS_SET_Hour(uint8_t hour, uint8_t address);
void DS_OnOff_Alarma1(uint8_t alarma);
void DS_OnOff_Alarma2(uint8_t alarma);
uint8_t DS_Status_Alarma1(void);
uint8_t DS_Status_Alarma2(void);
void DS_Set_Alarma(uint8_t hrs, uint8_t min, uint8_t aa);
void DS_Get_Alarma(uint8_t *hrs, uint8_t *min, uint8_t aa);
uint8_t DS_MoveUp_Horas(uint8_t hora);
uint8_t DS_MoveDown_Horas(uint8_t hora);
uint8_t DS_MoveUp_Min(uint8_t min);
uint8_t DS_MoveDown_Min(uint8_t min);

#endif	/* XC_HEADER_TEMPLATE_H */

