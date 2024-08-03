# I2C-with-PIC16F887

En este proyecto desarrollé la 'Libreria' de communicación I2C para el microcontrolador PIC16F887. En este repositorio encontrarás la comunicación en modo MASTER y SLAVE.
El microcontrolador en modo Maestro es capaz de controlar modulos y dispositivos como un DS1307 (módulo RTC), módulo PCF8574 (para controlar por I2C una pantalla LCD), otros
microcontroladores, etc. En modo Slave funciona como debería.

IMPORTANTE: Si usas la función Slave del microcontrolador PIC16F887 junto con un Maestro como la Raspberry Pi y Python, debes de desplazar la dirección del Slave un bit a la
derecha en tu código de programación en python. Ejemplo:

addres_pic = 0101 0000 // Dirección del PIC
correc_addres_in_pyton = 0010 1000 //Dirección del PIC en Python

Otra cosa a considerar es que uses resistencias PULL-UP en los pines correspondientes del microcontrolador, en este caso en RC3 y RC4 para el correcto funcionamiento de la
comunicación, de lo contrario, no funcionará. La resistencias recomendadas son de 4k a 10k. Particularmente uso resistencias de 10k.

![image](https://github.com/user-attachments/assets/4a355591-dadb-4c10-bfc7-a91ff8873572)
