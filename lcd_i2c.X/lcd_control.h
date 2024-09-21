/* #include "lcd_i2c.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 05/12/2019
* Electronica y Circuitos
*
*/

#ifndef LCD_I2C_H
#define	LCD_I2C_H

void LCDi2c_Init(void); //Si
void LCDi2c_CMD(unsigned char cmd); //Si
void LCDi2c_Set_Cursor(char col, char row); //Si
void LCDi2c_Write_Char(char c); //Si
void LCDi2c_Write_String(const char *str); //Si
void LCDi2c_Clear(void); //Si
void LCDi2c_Shift_Right(void); //Si
void LCDi2c_Shift_Left(void); //Si
void LCDi2c_Blink(void); //Si
void LCDi2c_NoBlink(void); //Si
void Lcd_CGRAM_WriteChar(char n);
void Lcd_CGRAM_CreateChar(char pos, const char* new_char);

#endif