<h1>5. ESTRUCTURA DE UN PROCESO</h1>

<h2>5.1 PROGRAMAS Y PROCESOS</h2>

Un programa es una colección de instrucciones y de datos que se encuentran almacenados en un archivo ordinario. Este archivo tiene en su nodo-i un atributo que lo identifica como ejecutable. Puede ser ejecutado por el propietario, el grupo y el resto de los usuarios, dependiendo de los permisos que tenga el archivo.


Los usuarios pueden crear archivos ejecutables de varias formas. Una de las sencillas es mediante la escritura de programa para el intérprete de órdenes (scripts de shell). Mediante este mecanismo se deben realizar dos pasos para obtener un programa. El primero es editar un archivo de texto que contenga una serie de líneas que puedan ser interpretadas por un intérprete de órdenes (`sh`, `csh` o `ksh`). Una vez editado el programa, hay que cambiar sus atributos para que sea ejecutable; esto se consigue con la orden `chmod`. Para poder ejecutar un archivo así creado, tendremos que arrancar un intérprete de órdenes, pasándole como parámetro el nombre de nuestro archivo de órdenes. Como, por defecto, al iniciar una sesión bajo UNIX, se ejecuta un intérprete para dar servicio al usuario, bastará con escribir el nombre del archivo de órdenes para que empiece a ejecutarse.

Trabajar con archivos de órdenes presenta grandes ventajas a la hora de realizar programas cortos que no sean de gran complejidad, pero es una seria limitación a la hora de afrontar el desarrollo de una aplicación de mayor tamaño. Para ello, en la mayor parte de las ocasiones, vamos a generar archivos ejecutables mediante la ayuda de lenguajes de alto o bajo nivel. En nuestro caso estamos empleando el compilador de lenguaje C.

Este mecanismo de generación ya ha sido estudiado, pero para ser exhaustivos lo vamos a exponer. Primero se debe crear un archivo de texto que contenga el código fuente de nuestro programa (este archivo tendría una extensión .c). El compilador de C (`gcc`) se va a encargar de traducir el código fuente a código objeto que entiende nuestra computadora. 

La estructura de todo programa ejecutable creado por el compilador de C viene impuesta por el sistema. Para ver una descripción detallada de esta estructura, podemos consultar la entrada a.out del manual de UNIX. A grosso modo, un programa consta de las siguientes partes:

<ul>
<li>Un conjunto de encabezados que describen los atributos del archivo.</li>
<li>Un bloque donde se encuentran las instrucciones en lenguaje máquina del programa. Este bloque se conoce en UNIX como texto del programa.</li>
<li>Un bloque dedicado a la representación en lenguaje máquina de los datos que deben ser inicializados cuando arranca la ejecución del programa. Aquí está incluida la indicación de cuánto espacio de memoria debe reservar el kernel para estos datos. Tradicionalmente, este bloque se conoce como `bss` (seudo-operador del ensamblador de IBM 7090 que significa `block started by symbol`). El kernel inicializa, en tiempo de ejecución, esta zona a valor 0.</li>
<li>Otras secciones, tales como tablas de símbolos.</li>
</ul>

Cuando un programa es leído del disco por el kernel y es cargado en memoria para ejecutarse, se convierte en un proceso. En un proceso no sólo hay una copia del programa, sino que además el kernel le añade información adicional para poder manejarlo.

Un proceso se compone de tres bloques fundamentales que se conocen como segmentos. Estos bloques son:

<ul>
<li>El segmento de texto. Contiene las instrucciones que entiende el CPU. Este bloque es una copia del bloque de texto del programa.</li>
<li>El segmento de datos. Contiene los datos que deben ser inicializados al arrancar el proceso. Si el programa ha sido generado por un compilador de C, en este bloque estarán las variables globales y las estáticas. Se corresponde con el bloque bss del programa.</li>
<li>El segmento de pila. Lo crea el kernel al arrancar el proceso y su tamaño es gestionado dinámicamente por el kernel. La pila se compone de una serie de bloques lógicos, llamados marcos de pila, que son introducidos cuando se llama a una función y son sacados cuando se vuelve de la función. Un marco de pila se compone de los parámetros de la función,  las variables locales de la función y la información necesaria para restaurar el marco de pila anterior a la llamada a la función (dentro de esta información se incluyen el contador de programa y el apuntador de pila anteriores a la llamada a la función). En los programas fuente no se incluye código para gestionar la pila (a menos que estén escritos en ensamblador); es el compilador quien incluye el código necesario para controlarlo.</li>
</ul>

Debido a que los procesos se ejecutan en dos modos: usuario y supervisor (también conocido como modo kernel); el sistema maneja dos pilas por separado. La pila del modo usuario contiene los argumentos, variables locales y otro datos relativos a funciones que se ejecutan en modo usuario, y la pila del modo supervisor contiene los marcos de pila de las funciones que se ejecutan en modo supervisor (estas funciones son las llamadas al sistema).

UNIX es un sistema que permite multiproceso (ejecución de varios procesos simultáneamente). El planificador es la parte del kernel encargada de gestionar el CPU y determinar qué proceso ocupa el tiempo de CPU en un determinado instante.

Un mismo programa puede estar siendo ejecutado en un instante determinado por varios procesos a la vez.

Desde un punto de vista funcional, un proceso en UNIX es la entidad que se crea tras la llamada `fork`. Todos los procesos, excepto el primero (proceso número 0), son creados mediante una llamada a `fork`. El proceso que llama a `fork` se conoce como proceso padre y el proceso creado es el proceso hijo. Todos los procesos tienen un único proceso padre, pero pueden tener varios procesos hijos. El kernel identifica cada proceso mediante su `PID` (process identification), que es un número asociado a cada proceso y que no cambia durante el tiempo de vida de éste.

El proceso 0 es especial; es creado cuando arranca el sistema, y después de hacer una llamada `fork` se convierte en el proceso intercambiador (encargado de la gestión de la memoria virtual). El proceso hijo creado se llama `init` y su `PID` vale 1. Este proceso es el encargado de arrancar los demás procesos del sistema según la configuración que se indica en el archivo `/etc/inittab`.

<h2>5.2 ESTADO DE UN PROCESO</h2>

El tiempo de vida de un proceso se puede dividir en un conjunto de estados, cada uno con unas características determinadas. En la siguiente figura podemos ver, en un primer nivel de aproximación, los estados por los que evoluciona un proceso. 

