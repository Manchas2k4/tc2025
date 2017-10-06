![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_a.png "Actividades")

# Actividad: Laboratorio - Actividad sobre sockets.

# Módulo: Programación de sockets.


## Descripción

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_b.png "Descripción") 
* Durante la sesión, de manera individual, deberás desarrollar la implementación ClienteServidor
del juego "Batalla Naval".

## Modalidad

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_c.png "Modalidad") 
* Individual.

## Objetivos de la actividad
* Utilizar las llamadas a sistema relacionadas con la programación de sockets.

## Instrucciones
  
![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_d.png "Instrucciones")
  
  ¿En qué consiste el juego? Se compone de dos tableros por jugador, dividido cada uno en
cuadrículas. Los tableros típicos son cuadrados de 10 por 10 casillas, y cada posición se
identifica con números para las columnas (de 1 a 10) y con letras para las filas (de la A a la
J). En uno de los tableros el jugador coloca sus barcos y registra los tiros del oponente. En el
otro, se registran los tiros propios.
Antes de comenzar, cada jugador posiciona los barcos de forma secreta o invisible al
oponente, generalmente con el tablero en posición vertical como pizarra. Cada uno ocupa,
según su modelo, una cierta cantidad de posiciones, ya sea horizontal o verticalmente. De
esta forma, no se permiten lugares solapados, ya que cada uno ocupa posiciones únicas.
Ambos participantes poseen y deben ubicar igual número de naves.
Una vez todas las naves han sido posicionadas, se inicia una serie de rondas. En cada ronda,
cada jugador en su turno indica una posición del tablero de su oponente. Si esa posición es
ocupada por una parte de un barco, el oponente indica averiado (toque o tocado) y el
atacante marca con rojo esa posición, con un pin. Cuando todas las posiciones de un mismo
barco han sido dañadas debe indicarse hundido dando a conocer tal circunstancia que
indicará al atacante la importancia de la nave destruida. Ahora bien, si la posición indicada,
efectivamente, no posee un barco alojado, se indica con agua, y será marcada con un pin
blanco.
Quien descubra primero todas las naves será el vencedor.
1. El servidor ITERATIVO tendrá el siguiente comportamiento:
Recibe de línea de comando la dirección IP y el puerto sobre el que va operar. El
programa deberá validar que se reciban esos datos.

2. Deberá esperar a que haya exactamente dos conexiones antes de continuar. El
servidor deberá llevar el control de los turnos, permitiendo que un jugador indique una
posición mientras el otro, espera.

3. Lo primero que el servidor realizará es pedir los “nicks”. Para ello le preguntará su
“nick” a un jugador mientras el otro, espera. En seguida, se intercambian los papeles.

4. Una vez realizado el punto anterior, el servidor deberá pedirle a un jugador su tirada y
comunicársela al otro jugador. El otro jugador deberá responder si el primero ha
acertado y si ya se han hundido todos sus barcos. A continuación se intercambian los
papales.

5. Este proceso continúa hasta que uno de los jugadores haya hundido todos los barcos
del otro. Al llegar a este punto, se termina el juego y la conexión.

**IMPORTANTE**: El servidor SOLO servirá para intercambiar mensajes entre los clientes, NO
DEBERÁ guardar ninguna información sobre la localización de los barcos.
1. El cliente tendrá el siguiente comportamiento:
Recibe de línea de comando la dirección IP y el puerto del servidor.
2. El cliente posee un tablero de 4x4 donde se “colocaran” los barcos que posee el
jugador. Antes de intentar cualquier comunicación, el programa deberá colocar 4
barcos al azar, desplegándose al usuario la posición en que queda cada barco.

3. En seguido, se establece la comunicación con el servidor y, dependiendo del mensaje,
puede:

* Si recibe la instrucción de mandar su “nick”, deberá pedir ese dato al usuario y
mandarlo.

* En otro caso espera a que le digan que puede proceder.

4. Y comienza el juego, y dependiendo de lo que indique el servidor, puede:
* Si la instrucción es mandar las coordenadas, deberá pedir los datos al usuario y
mandarlo. Espera la respuesta del servidor (si hay acertado o no y si ha
terminado el juego o no).
* Si la instrucción es esperar, deberá estar pendiente de recibir las coordenadas,
validar si existe un barco y mandar la respuesta adecuada (si hay acertado o no
y si ha terminado el juego o no).
Cuando recibe el mensaje de que ha terminado, cierra las conexiones y termina el
programa.

## Recursos

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_e.png "Recursos")
* No aplica.

## Especificaciones de entrega

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_f.png "Especificaciones")
* Enviar a través de la sección de "Tareas" de Blackboard.


<br>
<br>

**DR© Tecnológico de Monterrey Campus Querétaro| Departamento de Desarrollo Académico| México, 2012**
