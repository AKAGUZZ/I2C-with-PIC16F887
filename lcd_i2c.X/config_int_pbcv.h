/* #include "lcd_i2c.h"
*
* Creada por: Ing. Abiezer Hernandez O.
* Fecha de creacion: 05/12/2019
* Electronica y Circuitos
*
*/

#ifndef CONFIG_INT_PBCV
#define	CONFIG_INT_PBCV

//Prototipos
void cfginterrupciones(void); //M�dulo para configurar las interrupciones
void __interrupt() interrpExt(void); // M�dulo de atenci�n de interrepciones / Designa las tareas que se deben atender cada que ocurre una interrupci�n

#endif