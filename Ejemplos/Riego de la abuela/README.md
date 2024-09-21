En este proyecto desarrollé un prototipo, el cuál consistía en regar las plantas del jardín de mi abuela.
Este prototipo consideraba una pandalla LCD con su módulo de comunicación I2C para mostrar datos e información, además de un
módulo RTC (usé el DS1307), el cúal me daba la proporcionaba la hora cada vez que el pic lo consultaba.

La finalidad de este proyecto de ocio fue regar las plantas de mi abuela por la mañana y por la tarde-noche en una hora en
específico (de ahí la necesidad de usar el DS1307), por lo que tenía dos riegos programados que están por defecto a las
8:00 horas y a las 19:00 horas.

Logré que el módulo RTC me proporcionara los segundos, los minutos, las horas, el día de la semana, el día del mes, el mes y el
año. Esta información se mostraba en la pantalla LCD.

Para acceder a los distintos ménus y configurar los parámetros se emplearon PUSH BUTTOM y quedó de la siguiente manera.
RB0 = Menú (Para esto se emplearon interrupciones por cambio de valor en RBO, de esta manera si se pulsaban otros botones no
      sucedía nada)
RA0 = Cambio de valor ascedente (Para cambiar el valor de la hora o la fecha de manera ascendente)
R01 = Cambio de valor descendente (Para cambiar de valor de la hora o la fecha de manera descendente)
RA2 = Intercabiar entre segundos, minutos, horas o día, mes, año, día de la semana según sea el caso.

Estoy conciente que este proyecto falto mucho por desarrollar y por mejorar. Esto lo realicé durante mis ratos libres después
de acabar mi semestre en mi institución, pero, ahora estoy de vuelta al camino final de mi carrera buscando la titulación que
espero sea muy pronto. Espero poder retomar este proyecto y mejorarlo bastante, ya que, siento que algo a criticar es mi código.
Si alguien lo retoma, se que podrá mejorarlo y espero comparta sus mejoras y críticas.

Algunos de los problemas que enfrente fueron estos:
- Usé una bomba convecional de 5V que se emplean para proyectos escolares, por lo que no me daba la potencia que necesitaba.
- Me dio problemas la parte de potencia, por lo que recomiendo aislar de manera electrica el circuito de control o usar un
  filtro, esto puede ser con ayuda de un ocpoacoplador o un snubber. Yo usaba un relé para activar la bomba, pero los cambios
  de amperaje y voltaje al realizar la activación del relé ocasionaba que el microcontrlador se Reseteara.
