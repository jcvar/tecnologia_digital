Universidad Nacional De Colombia\
Tecnología Digital 2020-I

Grupo 4\
Juan Camilo Vargas `jcvargasq@unal.edu.co` \
Sergio Alejandro Vargas `savargasqu@unal.edu.co`

## Tarea — Sensores Análogos:

SensorA produce un valor entero valA de 0 a 300.

SensorB produce un valor entero valB de 0 a 200.

Exiten 4 leds indicadores: Red, Green, Yellow, Blue.

Condiciones para la activación de los LEDs:
- Si valA es mayor o igual a 250 se enciende el Red los demás apagados.
- Si valA es menor a 180 se tienen dos condiciones:
  - Si valB es inferior a valA en más de 30 y menos de 70 se enciende Yellow, los demás apagados.
  - Si valB es inferior en 70 o más se enciende Blue, los demás apagados.
- En cualquier otro caso se enciende Green y los demás apagados.

Mostrar 2 displays numéricos con los valores valA y valB y los 4 LEDs indicadores.
