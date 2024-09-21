/*
 * File:   ds1307_control.c
 * Author: guza_
 *
 * Created on 10 de junio de 2024, 01:31 PM
 */

#define _XTAL_FREQ 8000000
#include <xc.h>
#include "ds1307_control.h"
#include "master_i2c.h"
#include <stdint.h>
uint8_t horas, minutos, segundos, day, status_alarma1, status_alarma2;
uint8_t min_alarma1, hora_alarma1;
uint8_t min_alarma2, hora_alarma2;
uint8_t nibble_high, nibble_low;
void DS_SETHora(char set[]){
    horas = ((set[0] & 0x0F)<<4) + (set[1] &0x0F);
    minutos = ((set[2] & 0x0F)<<4) + (set[3] &0x0F);
    segundos = ((set[4] & 0x0F)<<4) + (set[5] &0x0F);
    __delay_ms(100);
    I2CMaster_Start();
    I2CMaster_Address(0xD0);
    I2CMaster_Write(0b00000000);
    I2CMaster_Write(segundos);
    I2CMaster_Write(minutos);
    I2CMaster_Write(horas);
    I2CMaster_Stop();
}

void DS_SET_Date(char set_fecha[]){
    horas = ((set_fecha[0] & 0x0F)<<4) + (set_fecha[1] &0x0F);
    minutos = ((set_fecha[2] & 0x0F)<<4) + (set_fecha[3] &0x0F);
    segundos = ((set_fecha[4] & 0x0F)<<4) + (set_fecha[5] &0x0F);
    day = ((set_fecha[6] & 0x07));
    __delay_ms(100);
    I2CMaster_Start();
    I2CMaster_Address(0xD0);
    I2CMaster_Write(0x03);
    I2CMaster_Write(day);
    I2CMaster_Write(horas);
    I2CMaster_Write(minutos);
    I2CMaster_Write(segundos);
    I2CMaster_Stop();
}

void DS_SET_Hour(uint8_t hour, uint8_t address){
    I2CMaster_Start();
    I2CMaster_Address(0xD0);
    I2CMaster_Write(address);
    I2CMaster_Write(hour);
    I2CMaster_Stop();
}

void DS_ReadHora(uint8_t *segundos, uint8_t *minutos, uint8_t *horas){
    I2CMaster_Start();
    I2CMaster_Address(0xD0);
    I2CMaster_Write(0x00);
    I2CMaster_Restart();
    I2CMaster_Address(0xD0+1);
    
    *segundos = I2CMaster_Read();
    I2CMaster_ACK();
    *minutos = I2CMaster_Read();
    I2CMaster_ACK();
    *horas = I2CMaster_Read();
    I2CMaster_NACK();
    I2CMaster_Stop();
}

void DS_ReadDate(uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year){
    I2CMaster_Start();
    I2CMaster_Address(0xD0);
    I2CMaster_Write(0x03);
    I2CMaster_Restart();
    I2CMaster_Address(0xD0+1);
    
    *day = I2CMaster_Read();
    I2CMaster_ACK();
    *date = I2CMaster_Read();
    I2CMaster_ACK();
    *month = I2CMaster_Read();
    I2CMaster_ACK();
    *year = I2CMaster_Read();
    I2CMaster_NACK();
    I2CMaster_Stop();
}

void DS_OnOff_Alarma1(uint8_t alarma){
    status_alarma1 = alarma;
}

void DS_OnOff_Alarma2(uint8_t alarma){
    status_alarma2 = alarma;
}

uint8_t DS_Status_Alarma1(void){
    return status_alarma1;
}

uint8_t DS_Status_Alarma2(void){
    return status_alarma2;
}

void DS_Set_Alarma(uint8_t hrs, uint8_t min, uint8_t aa){
    if(aa == 0x01){
        hora_alarma1 = hrs;
        min_alarma1 = min;
    }
    else if(aa == 0x02){
        hora_alarma2 = hrs;
        min_alarma2 = min;
    }
}

void DS_Get_Alarma(uint8_t *hrs, uint8_t *min, uint8_t aa){
    if(aa == 0x01){
        *hrs = hora_alarma1;
        *min = min_alarma1;
    }
    else if(aa == 0x02){
        *hrs = hora_alarma2;
        *min = min_alarma2;
    }
}

uint8_t DS_MoveUp_Horas(uint8_t hora){
    nibble_high = (hora & 0xF0) >> 4;
    nibble_low = hora & 0x0F;
    nibble_low += 1;
    if(nibble_low > 0x09){
        nibble_low = 0;
        nibble_high += 1;
    }
    if (nibble_high == 0x02 && nibble_low > 3){
        nibble_high = 0;
        nibble_low = 0;
    }
    hora = (nibble_high << 4) | (nibble_low & 0x0F);
    return hora;
}

uint8_t DS_MoveDown_Horas(uint8_t hora){
    nibble_high = (hora & 0xF0) >> 4;
    nibble_low = hora & 0x0F;

    if(nibble_low == 0) {
        nibble_low = 0x09;
        if(nibble_high > 0) {
            nibble_high -= 1;
        } else {
            nibble_high = 0x02;
            nibble_low = 0x03;
        }
    } else{
        nibble_low -= 1;
    }
    hora = (nibble_high << 4) | (nibble_low & 0x0F);
    return hora;
}

uint8_t DS_MoveUp_Min(uint8_t min){
    nibble_high = (min & 0xF0) >> 4;
    nibble_low = min & 0x0F;
    nibble_low += 1;
    if(nibble_low > 0x09){
        nibble_low = 0;
        nibble_high += 1;
        if (nibble_high > 0x05){
            nibble_low = 0;
            nibble_high = 0;
            
        }
    }
    min = (nibble_high << 4) | (nibble_low & 0x0F);
    return min;
}

uint8_t DS_MoveDown_Min(uint8_t min){
    nibble_high = (min & 0xF0) >> 4;
    nibble_low = min & 0x0F;

    if(nibble_low == 0) {
        nibble_low = 0x09;
        if(nibble_high > 0) {
            nibble_high -= 1;
        } else {
            nibble_high = 0x05;
            nibble_low = 0x09;
        }
    } else{
        nibble_low -= 1;
    }
    min = (nibble_high << 4) | (nibble_low & 0x0F);
    return min;
}