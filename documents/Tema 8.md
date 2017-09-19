# 8. Comunicación mediante tuberías
## 8.1 Comunicación entre procesos
La comunicación entre procesos va a habilitar mecanismos para que los procesos puedan intercambiarse datos y sincronizarse. Hasta ahora hemos visto dos formas bastantes elementales para que dos procesos se comuniquen: el envío de señales para la sincronización y el uso de archivos ordinarios. Las señales no deben considerarse parte de la forma habitual de comunicar dos procesos y su uso debe restringirse a la comunicación de eventos o situaciones excepcionales. Los archivos ordinarios tampoco son la forma más eficiente de comunicarse, ya que se ve involucrado el acceso a disco que suele ser 3 ó 4 órdenes de magnitud más lento que el acceso a memoria. 

Los mecanismos que vamos a estudiar a continuación pretenden dar soluciones más eficientes a las necesidades de comunicación empleando como canal de transmisión la memoria principal. Esto va a suponer una mayor velocidad de transferencia de datos.

A la hora de comunicar dos procesos, vamos a considerar dos situaciones claramente diferentes:
* Que los procesos se están ejecutando bajo el control de una misma computadora.
* Que los procesos se están ejecutando en computadoras separadas.

La primera situación no va a aportar especial dificultad, ya que lo que hemos estado realizando hasta ahora va en esa línea. Así, para comunicar dos o más procesos a nivel local, vamos a estudiar un mecanismo clásico como son las tuberías, y después vamos a pasar a ver las facilidades de IPC de UNIX. Estas facilidades están diseñadas con una misma filosofía y engloban tres mecanismos de comunicación: semáforos, memoria compartida y filas de mensajes.

El segundo escenario es más complejo, porque se ven involucradas las redes de computadoras y la comunicación entre ellas. Hoy en día este tema tiene mucho interés, porque el proceso distribuido es una tecnología con gran uso. La idea de tener varias computadoras interconectadas y que cualquier usuario pueda manejarlas como si se tratasen de una sola, pero con una potencia cercana a la suma de todas ellas, no sólo es interesante como ejercicio teórico, sino que ya tiene implementación real. 

Como introducción al tema de la comunicación entre computadoras remotas, vamos a exponer la interfaz que suministra el UNIX de Berkeley para comunicar procesos. Esta interfaz se compone de una serie de llamadas que manejan un nuevo tipo de archivo conocido como conector (socket) y que va a actuar como canal de comunicación entre procesos. Aunque un conector es tratado sintácticamente como un archivo; semánticamente, no lo es. Esto significa que no vamos a tener los problemas de velocidad inherentes al acceso a disco.

Las tuberías son una de las primeras formas de comunicación implantadas en UNIX y muchos sistemas se ofrecen hoy día con esta facilidad. 

Una tubería se puede considerar como un canal de comunicación entre dos procesos, y las hay de dos tipos: tuberías con nombre (`fifos`) y tuberías sin nombre.

## 8.2 Tuberías sin Nombre
Las tuberías sin nombre se crean con la llamada pipe y sólo el proceso que hace la llamada y sus descendientes pueden utilizarla. `pipe` tiene la siguiente declaración:

`int pipe (int filedes[2]);`

Si la llamada funciona correctamente, devolverá el valor 0 y creará una tubería sin nombre; en caso contrario, devolverá -1 y en `errno` estará el código del error producido.

La tubería creada la vamos a poder manejar a través del arreglo `filedes`. Los dos elementos de `filedes` se comportan como dos descriptores de archivo y los vamos a usar para escribir y leer de la tubería. Al escribir en `filedes[0]` vamos a extraer los datos de ella. Naturalmente, `filedes[1]` se comporta como un archivo de solo escritura y `filedes[0]` como un archivo de sólo lectura.

Como el núcleo trata la tubería igual que a un archivo del sistema, al crearla debe asignarle un nodo-i. También le asigna un par de descriptores de archivo (`filedes[0]` y `filedes[1]`) y reserva las correspondientes entradas en la tabla de archivos del sistema y en la tabla de descriptores del proceso.

Todo eso facilita el manejo de la tubería, ya que al recibir el mismo tratamiento que un archivo, vamos a poder escribir y leer de ella con las llamadas `write` y `read`, llamadas que empleamos para los archivos ordinarios, directorios, archivos especiales y también tuberías.

Los descriptores de archivo se heredan de padres a hijos tras la llamada a `fork` o a `exec`. Así, para que se comuniquen padre e hijo mediante una tubería, la abriremos en el padre y tanto padre como hijo podrán compartirla.

La sincronización entre los accesos de escritura y lectura la lleva a cabo el kernel, de tal manera que, las llamadas a `read` para sacar datos de la tubería, no van a devolver el control hasta que no haya datos escritos por otro proceso mediante la correspondiente llamada a `write`. También es el kernel el que se encarga de gestionar la tubería para dotarla de un mecanismo de acceso tipo `FIFO` (`first-in`, `first-out`) y así, el proceso receptor sacará los datos en el mismo orden en que los escriba el proceso emisor.

Los datos escritos en la tubería se gestionan en el buffer caché sin que llegue al disco, por lo que al producirse la transferencia a través de memoria, las tuberías constituyen un mecanismo de comunicación mucho más rápido que el uso de archivo ordinarios. El tamaño de una tubería; es decir, el bloque de datos más grande que podemos escribir en ella depende del sistema, pero se garantiza que no va a ser inferior a 4,096  byte.

Cuando la tubería está llena, las llamadas a `write` quedan desbloqueadas hasta que no se saquen suficientes datos de la tubería como para escribir el bloque deseado.

Como ejemplo de aplicación, vamos a ver la forma de enviar datos desde un proceso emisor a un proceso receptor a través de una tubería sin nombre (ver la siguiente figura). Este ejemplo es el mismo que vimos para ilustrar la sincronización mediante señales de dos procesos.  Ahora veremos que sólo tenemos que preocuparnos por los aspectos de envío y recepción, ya que la sincronización es algo que resuelve el kernel.

