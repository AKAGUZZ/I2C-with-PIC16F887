/*
 * File:   lcd_control.c
 * Author: guza_
 *
 * Created on 9 de junio de 2024, 06:07 PM
 */


#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdint.h>
#include "master_i2c.h"
#include "lcd_control.h"

void LCDi2c_Init(void){
    __delay_ms(20);
    LCDi2c_CMD(0x33);
    LCDi2c_CMD(0x32);
    LCDi2c_CMD(0x28);
    LCDi2c_CMD(0x0C);
    LCDi2c_CMD(0x06);
	LCDi2c_CMD(0x01);
    __delay_ms(3);
}

void LCDi2c_CMD(unsigned char cmd){
    char data_u, data_l;
	data_u = (cmd & 0xF0);
	data_l = ((cmd<<4) & 0xF0);
    I2CMaster_Start();
    I2CMaster_Address(0x4E);
    I2CMaster_Write(data_u|0x0C);
    I2CMaster_Write(data_u|0x08);
    I2CMaster_Write(data_l|0x0C);
    I2CMaster_Write(data_l|0x08);
    I2CMaster_Stop();
}

void LCDi2c_Set_Cursor(char col, char row){
    unsigned char address;
    switch(row)
    {
        case 1:
            address = 0x00;
            break;
        case 2:
            address = 0x40;
            break;
        case 3:
            address = 0x14;
            break;
        case 4:
            address = 0x54;
            break;
    }
    address += col - 1;
	LCDi2c_CMD(0x80 | address);
}

void LCDi2c_Write_Char(char c){
    char data_u, data_l;
	data_u = (c & 0xF0);
	data_l = ((c<<4) & 0xF0);
    I2CMaster_Start();
    I2CMaster_Address(0x4E);
    I2CMaster_Write(data_u|0x0D);
    I2CMaster_Write(data_u|0x09);
    I2CMaster_Write(data_l|0x0D);
    I2CMaster_Write(data_l|0x09);
    I2CMaster_Stop();
}

void LCDi2c_Write_String(const char *str){
    while(*str != '\0'){
        LCDi2c_Write_Char(*str++);
    }
}

void LCDi2c_Clear(void){
    LCDi2c_CMD(0x01);
    __delay_ms(2);
}

void LCDi2c_Shift_Right(void){
    LCDi2c_CMD(0x1C);
}

void LCDi2c_Shift_Left(void){
    LCDi2c_CMD(0x18);
}

void LCDi2c_Blink(void){
    LCDi2c_CMD(0x0F);
}

void LCDi2c_NoBlink(void){
    LCDi2c_CMD(0x0C);
}