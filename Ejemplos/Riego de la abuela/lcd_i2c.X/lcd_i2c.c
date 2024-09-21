/*
 * File:   lcd_i2c.c
 * Author: guza_
 *
 * Created on 9 de junio de 2024, 12:04 PM
 */


#include "config.h"
#include "master_i2c.h"
#include "lcd_control.h"
#include "config_int_pbcv.h"
#include "ds1307_control.h"
#include <stdio.h>
const char* dias[] = {"DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB"};
char tiempo[3];
uint8_t variable1, variable2, variable3, variable4;
uint8_t alarma1_hrs, alarma1_min, alarma2_hrs, alarma2_min;
uint8_t start_min, end_min, alto_nibble, bajo_nibble, aumento;
// char setter[] = "075959";

void main(void) {
    salidas();
    entradas();
    I2CMaster_init();
    LCDi2c_Init();
    cfginterrupciones();
    //DS_SETHora(setter);
    DS_OnOff_Alarma1(0x01);
    DS_OnOff_Alarma2(0x01);
    DS_Set_Alarma(0x08, 0x00, 0x01);
    DS_Set_Alarma(0x19, 0x00, 0x02);
    __delay_ms(100);
    
    while(1){
        DS_ReadDate(&variable1, &variable2, &variable3, &variable4);
        // SECCIÓN DE FECHA
        __delay_ms(10);
        LCDi2c_Set_Cursor(1, 1);
        LCDi2c_Write_String(dias[variable1-1]);

        __delay_ms(10);
        tiempo[0] = ((variable2 >> 4) & 0x07) + '0';
        tiempo[1] = ((variable2) & 0x0F) + '0';
        tiempo[2] = '\0';
        LCDi2c_Set_Cursor(5, 1);
        LCDi2c_Write_String(tiempo);

        __delay_ms(10);
        tiempo[0] = ((variable3 >> 4) & 0x07) + '0';
        tiempo[1] = ((variable3) & 0x0F) + '0';
        tiempo[2] = '\0';
        LCDi2c_Set_Cursor(8, 1);
        LCDi2c_Write_String(tiempo);

        __delay_ms(10);
        tiempo[0] = ((variable4 >> 4) & 0x07) + '0';
        tiempo[1] = ((variable4) & 0x0F) + '0';
        tiempo[2] = '\0';
        LCDi2c_Set_Cursor(11, 1);
        LCDi2c_Write_String(tiempo);
        
        DS_ReadHora(&variable1 , &variable2, &variable3);
        __delay_ms(10);
        
        // SECCIÓN DE HORAS
        tiempo[0] = ((variable3 >> 4) & 0x07) + '0';
        tiempo[1] = ((variable3) & 0x0F) + '0';
        tiempo[2] = '\0';
        LCDi2c_Set_Cursor(1, 2);
        LCDi2c_Write_String(tiempo);
        __delay_ms(10);
        
        LCDi2c_Set_Cursor(3, 2);
        LCDi2c_Write_String(":");
        tiempo[0] = ((variable2 >> 4) & 0x07) + '0';
        tiempo[1] = ((variable2) & 0x0F) + '0';
        tiempo[2] = '\0';
        LCDi2c_Set_Cursor(4, 2);
        LCDi2c_Write_String(tiempo);
        __delay_ms(10);
        
        LCDi2c_Set_Cursor(6, 2);
        LCDi2c_Write_String(":");
        tiempo[0] = ((variable1 >> 4) & 0x07) + '0';
        tiempo[1] = ((variable1) & 0x0F) + '0';
        tiempo[2] = '\0';
        LCDi2c_Set_Cursor(7, 2);
        LCDi2c_Write_String(tiempo);
        __delay_ms(10);
        
        
        if (DS_Status_Alarma1()) {
            DS_Get_Alarma(&alarma1_hrs, &alarma1_min, 0x01);
            if (alarma1_hrs == variable3 && alarma1_min == variable2) {
                for (unsigned char i = 1; i <=6; i++) {
                    // Regando durante 2 minutos
                    __delay_ms(100);
                    LCDi2c_Clear();
                    __delay_ms(100);
                    
                    LCDi2c_Set_Cursor(1, 1);
                    LCDi2c_Write_String("Regando...");
                    PORTD = (1<<0); // Activar RD0
                    start_min = variable2;
                    aumento = DS_MoveUp_Min(start_min);
                    end_min = DS_MoveUp_Min(aumento);

                    while (1) {
                        DS_ReadHora(&variable1, &variable2, &variable3);
                        __delay_ms(400);
                        if (variable2 == end_min) {
                            break;
                        }
                    }

                    // Descansando durante 2 minutos
                    __delay_ms(100);
                    LCDi2c_Clear();
                    __delay_ms(100);
                    
                    LCDi2c_Set_Cursor(1, 1);
                    LCDi2c_Write_String("Descansando...");
                    PORTD = (0<<0); // Desactivar RD0
                    start_min = variable2;
                    end_min = start_min;
                    
                    for(unsigned char i = 0; i<6; i++){
                        end_min = DS_MoveUp_Min(end_min);
                    }
                    
                    while (1) {
                        DS_ReadHora(&variable1, &variable2, &variable3);
                        __delay_ms(400);
                        if (variable2 == end_min) {
                            break;
                        }
                    }
                }
                // Parar riego después del tercer ciclo
                __delay_ms(100);
                LCDi2c_Clear();
                __delay_ms(100);
                LCDi2c_Set_Cursor(1, 1);
                LCDi2c_Write_String("Riego completado");
                PORTD = 0;
                __delay_ms(2000);
                LCDi2c_Clear();
                __delay_ms(100);
            }
        }
        
        
        if (DS_Status_Alarma2()) {
            DS_Get_Alarma(&alarma2_hrs, &alarma2_min, 0x02);
            if (alarma2_hrs == variable3 && alarma2_min == variable2) {
                for (unsigned char i = 1; i <= 6; i++) {
                    // Regando durante 2 minutos
                    __delay_ms(100);
                    LCDi2c_Clear();
                    __delay_ms(100);
                    
                    LCDi2c_Set_Cursor(1, 1);
                    LCDi2c_Write_String("Regando...");
                    PORTD = (1<<0); // Activar RD0
                    start_min = variable2;
                    aumento = DS_MoveUp_Min(start_min);
                    end_min = DS_MoveUp_Min(aumento);
                    
                    
                    while (1) {
                        DS_ReadHora(&variable1, &variable2, &variable3);
                        __delay_ms(400);
                        if (variable2 == end_min) {
                            break;
                        }
                    }

                    // Descansando durante 2 minutos
                    
                    __delay_ms(100);
                    LCDi2c_Clear();
                    __delay_ms(100);
                    LCDi2c_Set_Cursor(1, 1);
                    LCDi2c_Write_String("Descansando...");
                    PORTD = (0<<0);
                    start_min = variable2;
                    end_min = start_min;
                    
                    for(unsigned char i = 0; i<6; i++){
                        end_min = DS_MoveUp_Min(end_min);
                    }

                    while (1) {
                        DS_ReadHora(&variable1, &variable2, &variable3);
                        __delay_ms(400);
                        if (variable2 == end_min) {
                            break;
                        }
                    }
                }
                // Parar riego después del tercer ciclo
                __delay_ms(100);
                LCDi2c_Clear();
                __delay_ms(100);
                
                LCDi2c_Set_Cursor(1, 1);
                LCDi2c_Write_String("Riego completado");
                PORTD = 0x00;
                __delay_ms(2000);
                LCDi2c_Clear();
                __delay_ms(100);
            }
        }
        
    }
    return;
}