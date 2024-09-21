/*
 * File:   config_int_pbcv.c
 * Author: guza_
 *
 * Created on 9 de junio de 2024, 11:41 PM
 */

#define _XTAL_FREQ 8000000
#include <xc.h>
#include "config_int_pbcv.h"
#include "lcd_control.h"
#include "ds1307_control.h"
//Definiciones
unsigned char contador = 0;
unsigned char salir = 0;
unsigned char cont = 0;
uint8_t var1, var2, var3, var4, address, var5, lim_sup_nl, lim_inf_nl, lim_sup_nh, lim_inf_nh, reinicio;
char contenedor[3];
uint8_t nibble_alto, nibble_bajo, a11, a12, a21, a22, escribir1, escribir2;
const char* dias[] = {"DOM", "LUN", "MAR", "MIE", "JUE", "VIE", "SAB"};

void cfginterrupciones(void)
{
    INTCON |= (1<<7); //Habilitamos las interrupciones globales
    INTCON |= (1<<4); //Habilitamos la interrupción externa en RB0
    INTCON &= ~(1<<1); //Limpiamos la bandera de interrupción externa
    OPTION_REG |= (1<<6); //Habilitamos lectura de flancos de subida
}

void __interrupt() interrpExt(void) //Atiende la interrupción
{
    if(INTCONbits.INTF){
        while (1){
            __delay_ms(10);
            LCDi2c_Clear();
            __delay_ms(10);
            while (!PORTBbits.RB0){                
                // Sección de Información de hora de riego
                if (salir == 1){
                    DS_Get_Alarma(&a11, &a12, 0x01);
                    DS_Get_Alarma(&a21, &a22, 0x02);
                    LCDi2c_Set_Cursor(1, 1);
                    LCDi2c_Write_String("Hora de Riego");
                    __delay_ms(10);
                    
                    __delay_ms(10);
                    contenedor[0] = ((a11 >> 4) & 0x07) + '0';
                    contenedor[1] = ((a11) & 0x0F) + '0';
                    contenedor[2] = '\0';
                    LCDi2c_Set_Cursor(1, 2);
                    LCDi2c_Write_String(contenedor);
                    __delay_ms(5);
                    
                    LCDi2c_Set_Cursor(3, 2);
                    LCDi2c_Write_String(":");
                    contenedor[0] = ((a12 >> 4) & 0x07) + '0';
                    contenedor[1] = ((a12) & 0x0F) + '0';
                    contenedor[2] = '\0';
                    LCDi2c_Set_Cursor(4, 2);
                    LCDi2c_Write_String(contenedor);
                    __delay_ms(5);
                    
                    LCDi2c_Set_Cursor(7, 2);
                    LCDi2c_Write_String("y");
                    __delay_ms(5);
                    
                    contenedor[0] = ((a21 >> 4) & 0x07) + '0';
                    contenedor[1] = ((a21) & 0x0F) + '0';
                    contenedor[2] = '\0';
                    LCDi2c_Set_Cursor(9, 2);
                    LCDi2c_Write_String(contenedor);
                    __delay_ms(5);
                    
                    LCDi2c_Set_Cursor(11, 2);
                    LCDi2c_Write_String(":");
                    contenedor[0] = ((a22 >> 4) & 0x07) + '0';
                    contenedor[1] = ((a22) & 0x0F) + '0';
                    contenedor[2] = '\0';
                    LCDi2c_Set_Cursor(12, 2);
                    LCDi2c_Write_String(contenedor);
                    __delay_ms(5);
                    
                    while(1){
                        __delay_ms(350);
                        if(PORTBbits.RB0){
                            salir = 2;
                            break;
                        }
                        contador += 1;
                        if (contador > 14){
                            salir = 10;
                            break;
                        }
                    }
                }
                __delay_ms(200);
                if (contador > 12){
                    break;
                }
                
                // Sección de selección de configuración
                
                
                switch(salir){
                    case 2:
                        __delay_ms(10);
                        LCDi2c_Clear();
                        __delay_ms(10); 
                        LCDi2c_Set_Cursor(1, 1);
                        switch(cont){
                            case 0:
                                LCDi2c_Write_String("Editar horas");
                                break;
                            case 1:
                                LCDi2c_Write_String("Editar minutos");
                                break;
                            case 2:
                                LCDi2c_Write_String("Editar segundos");
                                break;
                        }
                        
                        while(1){
                            // Sección de lecura de hora
                            DS_ReadHora(&var1, &var2, &var3);
                            contenedor[0] = ((var3 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var3) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(1, 2);
                            LCDi2c_Write_String(contenedor);
                            __delay_ms(5);
                            
                            LCDi2c_Set_Cursor(3, 2);
                            LCDi2c_Write_String(":");
                            contenedor[0] = ((var2 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var2) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(4, 2);
                            LCDi2c_Write_String(contenedor);
                            __delay_ms(5);
                            
                            LCDi2c_Set_Cursor(6, 2);
                            LCDi2c_Write_String(":");
                            contenedor[0] = ((var1 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var1) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(7, 2);
                            LCDi2c_Write_String(contenedor);
                            __delay_ms(5);
                            
                            // Sección de Ajuste
                            if (cont == 0){
                                if(PORTAbits.RA0){
                                    nibble_alto = (var3 & 0xF0) >> 4;
                                    nibble_bajo = var3 & 0x0F;
                                    nibble_bajo += 1;
                                    if(nibble_bajo > 0x09){
                                        nibble_bajo = 0;
                                        nibble_alto += 1;
                                        if (nibble_alto == 0x02){
                                            if (nibble_bajo > 3){
                                                nibble_alto = 0;
                                                nibble_bajo = 0;
                                            }
                                        }
                                    }
                                    var3 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var3, 0x02);
                                    __delay_ms(350);
                                }

                                if(PORTAbits.RA1){
                                    nibble_alto = (var3 & 0xF0) >> 4;
                                    nibble_bajo = var3 & 0x0F;

                                    if(nibble_bajo == 0) {
                                        nibble_bajo = 0x09;
                                        if(nibble_alto > 0) {
                                            nibble_alto -= 1;
                                        } else {
                                            nibble_alto = 0x02;
                                            nibble_bajo = 0x03;
                                        }
                                    } else{
                                        nibble_bajo -= 1;
                                    }
                                    var3 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var3, 0x02);
                                    __delay_ms(350);
                                }
                            }
                            
                            if (cont == 1 || cont == 2){
                                if (cont == 1){
                                    address = 0x01;
                                    var5 = var2;
                                    
                                }
                                else if (cont == 2){
                                    address = 0x00;
                                    var5 = var1;
                                }
                                
                                if(PORTAbits.RA0){
                                    nibble_alto = (var5 & 0xF0) >> 4;
                                    nibble_bajo = var5 & 0x0F;
                                    nibble_bajo += 1;
                                    if(nibble_bajo > 0x09){
                                        nibble_bajo = 0;
                                        nibble_alto += 1;
                                        if (nibble_alto == 0x05){
                                            if (nibble_bajo > 0x09){
                                                nibble_alto = 0;
                                                nibble_bajo = 0;
                                            }
                                        }
                                    }
                                    var5 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var5, address);
                                    __delay_ms(350);
                                }
                                
                                if(PORTAbits.RA1){
                                    nibble_alto = (var5 & 0xF0) >> 4;
                                    nibble_bajo = var5 & 0x0F;

                                    if(nibble_bajo == 0) {
                                        nibble_bajo = 0x09;
                                        if(nibble_alto > 0) {
                                            nibble_alto -= 1;
                                        } else {
                                            nibble_alto = 0x05;
                                            nibble_bajo = 0x09;
                                        }
                                    } else{
                                        nibble_bajo -= 1;
                                    }
                                    var5 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var5, address);
                                    __delay_ms(350);
                                }
                            }
                            
                            //Sección de rompimiento de ciclo
                            if (PORTBbits.RB0){
                                break;
                            }
                            if (PORTAbits.RA2){
                                salir = 2;
                                cont += 1;
                                if (cont > 2){
                                    cont = 0;
                                }
                                __delay_ms(400);
                                break;
                            }
                        }
                        break;
                    case 3:
                        __delay_ms(10);
                        LCDi2c_Clear();
                        __delay_ms(10);
                        LCDi2c_Set_Cursor(1, 1);
                        
                        switch(cont){
                            case 0:
                                address = 0x03;
                                LCDi2c_Write_String("Edit dia sem");
                                break;
                            case 1:
                                address = 0x04;
                                lim_sup_nl = 0x09;
                                lim_inf_nl = 0x00; // No se usa
                                lim_sup_nh = 0x03;
                                lim_inf_nh = 0x00; // No se usa
                                reinicio = 0x01;
                                LCDi2c_Write_String("Editar dia");
                                break;
                            case 2:
                                address = 0x05;
                                lim_sup_nl = 0x09;
                                lim_sup_nh = 0x01;
                                reinicio = 0x02;
                                LCDi2c_Write_String("Editar mes");
                                break;
                            case 3:
                                address = 0x06;
                                lim_sup_nl = 0x09;
                                lim_sup_nh = 0x09;
                                reinicio = 0x09;
                                LCDi2c_Write_String("Editar anio");
                                break;
                        }
                        
                        while(1){
                            DS_ReadDate(&var1, &var2, &var3, &var4);
                            
                            LCDi2c_Set_Cursor(1, 2);
                            LCDi2c_Write_String(dias[var1-1]);
                            
                            __delay_ms(10);
                            contenedor[0] = ((var2 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var2) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(5, 2);
                            LCDi2c_Write_String(contenedor);

                            __delay_ms(10);
                            contenedor[0] = ((var3 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var3) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(8, 2);
                            LCDi2c_Write_String(contenedor);

                            __delay_ms(10);
                            contenedor[0] = ((var4 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var4) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(11, 2);
                            LCDi2c_Write_String(contenedor);
                            
                            //Ajuste de día de la semana
                            if (cont == 0){
                                if (PORTAbits.RA0){
                                    nibble_alto = (var1 & 0xF0) >> 4;
                                    nibble_bajo = var1 & 0x0F;
                                    nibble_bajo += 1;
                                    if(nibble_bajo > 0x07){
                                        nibble_bajo = 1;
                                    }
                                    var1 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var1, address);
                                    __delay_ms(350);
                                }

                                if(PORTAbits.RA1){
                                    nibble_alto = (var1 & 0xF0) >> 4;
                                    nibble_bajo = var1 & 0x0F;
                                    if(nibble_bajo == 1) {
                                        nibble_bajo = 0x07;
                                    } else{
                                        nibble_bajo -= 1;
                                    }
                                    var1 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var1, address);
                                    __delay_ms(350);
                                }
                            }
                            
                            //Ajuste de día del mes
                            if (cont >= 1 && cont <= 3){
                                if (cont == 1){
                                    var5 = var2;
                                } 
                                else if (cont == 2){
                                    var5 = var3;
                                }
                                else if (cont == 3){
                                    var5 = var4;
                                }
                                if (PORTAbits.RA0){
                                    nibble_alto = (var5 & 0xF0) >> 4;
                                    nibble_bajo = var5 & 0x0F;
                                    nibble_bajo += 1;
                                    if(nibble_bajo > lim_sup_nl){
                                        nibble_bajo = 0;
                                        nibble_alto += 1;
                                    }
                                    if (nibble_alto == lim_sup_nh && nibble_bajo > reinicio){
                                        nibble_bajo = 0x01;
                                        nibble_alto = 0;
                                    }
                                    var5 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var5, address);
                                    __delay_ms(350);
                                }
                                
                                if(PORTAbits.RA1){
                                    nibble_alto = (var5 & 0xF0) >> 4;
                                    nibble_bajo = var5 & 0x0F;
                                    if(nibble_bajo == reinicio && nibble_alto == 0){
                                        nibble_alto = lim_sup_nh;
                                        nibble_bajo = reinicio;
                                    }
                                    else if (nibble_bajo == 0x00 && nibble_alto > 0x00){
                                        nibble_bajo = lim_sup_nl;
                                        nibble_alto -= 1;
                                    } else{
                                        nibble_bajo -= 1;
                                    }
                                    var5 = (nibble_alto << 4) | (nibble_bajo & 0x0F);
                                    DS_SET_Hour(var5, address);
                                    __delay_ms(350);
                                }
                            }
                            
                            //Sección de rompimiento de ciclo
                            if (PORTBbits.RB0){
                                break;
                            }
                            if (PORTAbits.RA2){
                                salir = 3;
                                cont += 1;
                                if (cont > 3){
                                    cont = 0;
                                }
                                __delay_ms(400);
                                break;
                            }
                        }
                        break;
                    case 4:
                        __delay_ms(10);
                        LCDi2c_Clear();
                        __delay_ms(10);
                        if (contador > 1){
                            contador = 0;
                        }
                        LCDi2c_Set_Cursor(1, 1);
                        switch(contador){
                            case 0:
                                LCDi2c_Write_String("Riego 1");
                                __delay_ms(10);
                                while(1){
                                    if (DS_Status_Alarma1() == 1){
                                        LCDi2c_Set_Cursor(1, 2);
                                        __delay_ms(10);
                                        LCDi2c_Write_String("ON ");
                                    } else{
                                        LCDi2c_Set_Cursor(1, 2);
                                        __delay_ms(10);
                                        LCDi2c_Write_String("OFF");
                                    }
                                    
                                    if (PORTAbits.RA0 || PORTAbits.RA1){
                                        //var5 = DS_Status_Alarma1();
                                        DS_OnOff_Alarma1(DS_Status_Alarma1() ^ 0x01);
                                        __delay_ms(350);
                                    }
                                    
                                    if (PORTAbits.RA2){
                                        contador = 1;
                                        __delay_ms(350);
                                        break;
                                    }
                                    if (PORTBbits.RB0){
                                        __delay_ms(350);
                                        salir = 5;
                                        break;
                                    }
                                }
                                break;
                            case 1:
                                LCDi2c_Write_String("Riego 2");
                                __delay_ms(10);
                                while(1){
                                    if (DS_Status_Alarma2() == 1){
                                        LCDi2c_Set_Cursor(1, 2);
                                        __delay_ms(10);
                                        LCDi2c_Write_String("ON ");
                                    } else{
                                        LCDi2c_Set_Cursor(1, 2);
                                        __delay_ms(10);
                                        LCDi2c_Write_String("OFF");
                                    }
                                    if (PORTAbits.RA0 || PORTAbits.RA1){
                                        DS_OnOff_Alarma2(DS_Status_Alarma2() ^ 0x01);
                                        __delay_ms(350);
                                    }
                                    
                                    if (PORTAbits.RA2){
                                        contador = 0;
                                        __delay_ms(350);
                                        break;
                                    }
                                    if (PORTBbits.RB0){
                                        __delay_ms(350);
                                        salir = 5;
                                        break;
                                    }
                                }
                                contador = 0;
                                break;
                        }                        
                        break;
                    case 5:
                        __delay_ms(10);
                        LCDi2c_Clear();
                        __delay_ms(10);
                        LCDi2c_Set_Cursor(1, 1);
                        LCDi2c_Write_String("Edit Riego 1");
                        __delay_ms(10);
                        contador  = 0;
                        while(1){
                            DS_Get_Alarma(&var1, &var2, 0x01);                          
                            contenedor[0] = ((var1 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var1) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(1, 2);
                            LCDi2c_Write_String(contenedor);
                            __delay_ms(5);
                            
                            LCDi2c_Set_Cursor(3, 2);
                            LCDi2c_Write_String(":");
                            contenedor[0] = ((var2 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var2) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(4, 2);
                            LCDi2c_Write_String(contenedor);
                            __delay_ms(5);
                            
                            if (contador == 0){
                                LCDi2c_Set_Cursor(14, 2);
                                LCDi2c_Write_String("HRS");
                                __delay_ms(5);
                            } else{
                                LCDi2c_Set_Cursor(14, 2);
                                LCDi2c_Write_String("MIN");
                                __delay_ms(5);
                            }
                            
                            if (PORTAbits.RA2){
                                contador = contador ^ 0x01;
                                __delay_ms(350);
                            }
                            
                            if (PORTAbits.RA0){
                                if(contador == 0){
                                    var5 = var1;
                                    var5 = DS_MoveUp_Horas(var5);
                                    DS_Set_Alarma(var5, var2, 0x01);
                                    __delay_ms(350);
                                } else {
                                    var5 = var2;
                                    var5 = DS_MoveUp_Min(var5);
                                    DS_Set_Alarma(var1, var5, 0x01);
                                    __delay_ms(350);
                                }
                            }
                            if(PORTAbits.RA1){
                                if(contador == 0){
                                    //var5 = var1;
                                    var5 = DS_MoveDown_Horas(var1);
                                    DS_Set_Alarma(var5, var2, 0x01);
                                    __delay_ms(350);
                                }
                                else{
                                    //var5 = var2;
                                    var5 = DS_MoveDown_Min(var2);
                                    DS_Set_Alarma(var1, var5, 0x01);
                                    __delay_ms(350);
                                }
                            }
                            
                            if(PORTBbits.RB0){
                                salir = 6;
                                __delay_ms(350);
                                break;
                            }
                        }
                        break;
                    case 6:
                        __delay_ms(10);
                        LCDi2c_Clear();
                        __delay_ms(10);
                        LCDi2c_Set_Cursor(1, 1);
                        LCDi2c_Write_String("Edit Riego 2");
                        __delay_ms(10);
                        contador = 0;
                        while(1){
                            DS_Get_Alarma(&var1, &var2, 0x02);                          
                            contenedor[0] = ((var1 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var1) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(1, 2);
                            LCDi2c_Write_String(contenedor);
                            __delay_ms(5);
                            
                            LCDi2c_Set_Cursor(3, 2);
                            LCDi2c_Write_String(":");
                            contenedor[0] = ((var2 >> 4) & 0x07) + '0';
                            contenedor[1] = ((var2) & 0x0F) + '0';
                            contenedor[2] = '\0';
                            LCDi2c_Set_Cursor(4, 2);
                            LCDi2c_Write_String(contenedor);
                            __delay_ms(5);
                            
                            if (contador == 0){
                                LCDi2c_Set_Cursor(14, 2);
                                LCDi2c_Write_String("HRS");
                                __delay_ms(5);
                            } else{
                                LCDi2c_Set_Cursor(14, 2);
                                LCDi2c_Write_String("MIN");
                                __delay_ms(5);
                            }
                            
                            if (PORTAbits.RA2){
                                contador = contador ^ 0x01;
                                __delay_ms(350);
                            }
                            
                            if (PORTAbits.RA0){
                                if(contador == 0){
                                    var5 = DS_MoveUp_Horas(var1);
                                    DS_Set_Alarma(var5, var2, 0x02);
                                    __delay_ms(350);
                                } else {
                                    var5 = DS_MoveUp_Min(var2);
                                    DS_Set_Alarma(var1, var5, 0x02);
                                    __delay_ms(350);
                                }
                            }
                            if(PORTAbits.RA1){
                                if(contador == 0){
                                    var5 = DS_MoveDown_Horas(var1);
                                    DS_Set_Alarma(var5, var2, 0x02);
                                    __delay_ms(350);
                                }
                                else{
                                    var5 = DS_MoveDown_Min(var2);
                                    DS_Set_Alarma(var1, var5, 0x02);
                                    __delay_ms(350);
                                }
                            }
                            
                            if(PORTBbits.RB0){
                                salir = 7;
                                __delay_ms(350);
                                break;
                            }
                        }
                        break;
                }
                if(salir == 7){
                    break;
                }
            }
            __delay_ms(400);
            salir += 1;
            if (salir > 7){
                __delay_ms(10);
                LCDi2c_Clear();
                __delay_ms(10);
                salir = 0;
                break;
            }
        }
        cont = 0;
        contador = 0;
        salir = 0;
    }
    INTCON &= ~(1<<1);
}