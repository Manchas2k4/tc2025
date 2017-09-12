<h1>INTRODUCCIÓN</h1>

<h2>1.1 ESTRUCTURA DEL SISTEMA</h2>

<p>Unix/Linux es un sistema operativo de tiempo compartido. El núcleo del sistema (kernel) es un programa que siempre está residente en memoria y, entre otros, brinda los siguientes servicios:</p>

<ul>
  <li>Controla los recursos del hardware.</li>
  <li>Controla los dispositivos periféricos (discos, terminales, impresoras, etc.).</li>
  <li>Permite a distintos usuarios compartir recursos y ejecutar sus programas.</li>
  <li>Proporciona un sistema de archivos que administra el almacenamiento de información (programas, datos, documentos, etc.).</li>
</ul>

  <p>En un esquema más amplio, Unix/Linux abarca también un conjunto de programas estándar, como pueden ser:</p>
  
<ul>
  <li>Compiladores de lenguajes.</li>
  <li>Editores de texto.</li>
  <li>Intérpretes de órdenes.</li>
  <li>Programas de gestión de archivos y directorios.</li>
</ul>

<p>Unix/Linux puede ser dividido en tres capas:</p> 

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/github1.png)

<p>El nivel más interno no pertenece realmente al sistema operativo, si no que es el hardware, la máquina sobre la que está implementado el sistema y cuyos recursos queremos gestionar.</p>

<p>Directamente en contacto con el hardware se encuentra el kernel, el cual está escrito en el lenguaje C en su mayor parte, aunque coexistiendo con código ensamblador. Las primeras implementaciones de Unix se hicieron en lenguaje ensamblador, pero en 1973 Ritchie reescribió el sistema en lenguaje C el cual se había desarrollado para recodificar Unix de una forma independiente de la máquina.</p>

<p>En el tercer nivel de nuestra estructura se encuentran programas estándar de cualquier sistema Unix/Linux (vi, grep, sh, who, etc.) y programas generados por el usuario. Hay que hacer notar que estos programas del tercer nivel nunca van a actuar sobre el hardware de forma directa. Para esto existe un mecanismo que nos permite indicarle al kernel que necesitamos operar sobre determinados recursos de hardware. Este mecanismo es lo que se conoce como “llamadas al sistema” (system calls) y es el objeto principal de este curso. Así pues, cualquier programa que se esté ejecutando bajo el control de Unix/Linux, cuando necesite hacer uso de alguno de los recursos que le brinda el sistema, deberá efectuar una llamada a alguna de las “system calls”.</p>

<p>La jerarquía de programas no tiene porqué verse limitada a tres niveles. El usuario puede crear tantos niveles como necesite. Puede haber también programas que se apoyen en diferentes niveles y que se comuniquen con el kernel por un lado, y con otros programas existentes por otro.</p>

<h2>1.2 ARQUITECTURA DEL SISTEMA OPERATIVO UNIX/LINUX</h2>

<p>A continuación esbozaremos, desde un punto vista muy general, los bloques funcionales básicos de que consta el núcleo de Unix/Linux.</p>

<p>Los dos conceptos centrales sobre los que se basa la arquitectura de Unix/Linux son los archivos y los procesos. El kernel está pensando para facilitarnos servicios relacionados con el sistema de archivos y con el control de procesos.</p>

<p>La siguiente figura muestra los tres niveles que vamos a estudiar en la arquitectura del sistema: hardware, kernel y usuario.</p>

<p>Las llamadas al sistema y su biblioteca asociada representan la frontera entre los programas del usuario y el kernel. La biblioteca asociada a las llamadas es el mecanismo mediante el cual podemos invocar una llamada desde un programa C/C++. Esta biblioteca se lanza por defecto al compilar cualquier programa C/C++ y se encuentra en el archivo /usr/lib/libc.a. Los programas escritos en lenguaje ensamblador pueden invocar directamente a las llamadas al sistema sin necesidad de ninguna biblioteca intermedia.</p>

<p>Las llamadas al sistema se ejecutan en modo kernel (o modo supervisor) y para entrar en este modo hay que ejecutar una sentencia en código máquina como trap (o interrupción de software). Es por esto que las llamadas al sistema pueden ser invocadas  directamente desde ensamblador y no desde C/C++.</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/imagentema1.png)

<p>En la figura anterior podemos apreciar que el núcleo está dividido en dos subsistemas principales: subsistema de archivos y subsistema de control de procesos.</p>

<p>El subsistema de archivos controla los recursos del sistema de archivos y tiene funciones como reservar el espacio para los archivos, administrar el espacio libre, controlar el acceso a los archivos, permitir el intercambio de datos entre los archivos y el usuario, etc. Los procesos interactúan con el subsistema de archivos a través de unas llamadas específicas.</p>

<p>El subsistema de archivos se comunica con los dispositivos de almacenamiento secundario (discos duros, unidades de cinta, etc.) a través de los manejadores de dispositivo (device drivers). Los manejadores de dispositivo se encargan de proporcionar el protocolo de comunicación (handshake) entre el núcleo y los periféricos. Se consideran dos tipos de dispositivos según la forma de acceso: dispositivos modo bloque (block devices) y dispositivos modo carácter (row devices). El acceso a los dispositivos en modo bloque se lleva a cabo con la intervención de buffers que mejoran enormemente la velocidad de transferencia. El acceso a dispositivos en modo carácter se lleva a cabo de forma directa, sin la intervención de buffers. Un mismo dispositivo físico puede ser manejado tanto en modo bloque como en modo carácter, dependiendo de qué manejador usemos para acceder a él.</p>

<p>El subsistema de control de procesos es el responsable de la planificación de los procesos (scheduler), su sincronización, comunicación entre los mismos (IPC – Inter Process Communication) y del control de la memoria principal.</p>

<p>El módulo de gestión de memoria se encarga de controlar qué procesos están cargados en la memoria principal en todo momento. Si en un momento determinado no hay suficiente memoria principal para todos los procesos que lo solicitan, el gestor de memoria debe recurrir a mecanismos de intercambio (swapping) para que todos los procesos tengan derecho a un tiempo mínimo de ocupación de la memoria y se puedan ejecutar.</p>

<p>El intercambio consiste en llevar los procesos cuyo tiempo de ocupación de la memoria expira a una memoria secundaria (que generalmente es el área que se dedica a intercambio en el disco – área de swap- y que se monta como un sistema de archivos aparte) y traer de esa memoria secundaria los procesos a los que se les asigna tiempo de ocupación de la memoria principal. Al módulo gestor de memoria se le conoce también como intercambiador (swapper).</p>

<p>El planificador o scheduler se encarga de gestionar el tiempo de CPU que tiene asignado cada proceso. El scheduler entra en ejecución cada cierto tiempo y decide si el proceso actual tiene derecho a seguir ejecutándose (esto depende de su prioridad y de sus privilegios) o ha de cambiarse de contexto (asignarle el CPU a otro proceso).</p>

<p>La comunicación entre procesos puede realizarse de forma asíncrona (señales) o síncrona (colas de mensaje, semáforos).</p>

<p>Por último, el control del hardware es la parte del núcleo encargada del manejo de las interrupciones y de la comunicación con la máquina. Los dispositivos pueden interrumpir al CPU mientras se está ejecutando un proceso. Si esto ocurre, el núcleo debe reanudar la ejecución del proceso después de atender a la interrupción. Las interrupciones no son atendidas por procesos, sino por funciones especiales, codificadas en el núcleo, que son invocadas durante la ejecución de cualquier proceso.</p>

<h1>2. ARQUITECTURA DEL SISTEMA DE ARCHIVOS</h1>

<h2>2.1 CARACTERÍSTICAS DEL SISTEMA DE ARCHIVOS</h2>

<p>El sistema de archivos de Unix/Linux se caracteriza por:</p>

<ul>
<li>Poseer una estructura jerárquica.</li>
<li>Realizar un tratamiento consistente de los datos de los archivos.</li>
<li>Poder crear y borrar archivos.</li>
<li>Permitir un crecimiento dinámico de los archivos.</li>
<li>Tratar los dispositivos y periféricos (terminales, unidades de disco, etc.) como si fueran archivos.</li>
</ul>

<p>El sistema de archivos está organizado, a nivel lógico, en forma de árbol, con un nodo principal conocido como raíz (“/”). Cada nodo dentro del árbol es un directorio que puede contener a su vez otros nodos (subdirectorios), archivos normales o archivos de dispositivos.</p>

<p>Los programas que se ejecutan en Unix/Linux no conocen el formato interno con el que el kernel almacena los datos. Cuando accedemos al contenido de un archivo mediante una llamada al sistema (read), el sistema nos lo va a presentar cómo una secuencia de bytes sin formato. Nuestro programa será el encargado de interpretar la secuencia de bytes y darle un significado según sus necesidades. Por lo tanto, la sintaxis (forma) del acceso a los datos de un archivo viene impuesta por el sistema y es la misma para todos los programas, y la semántica (significado) de los datos es responsabilidad del programa que trabaja con el archivo.</p>

<h2>2.2 ESTRUCTURA DEL SISTEMA DE ARCHIVOS</h2>

<p>Los sistemas de archivos suelen estar situados en dispositivos de almacenamiento modo bloque, tales como cintas o discos.</p>


<p>Las cintas tienen un tiempo de acceso mucho más alto que los discos, por ello, es poco práctico instalar un sistema de archivos sobre ellas. Sin embargo, son muy útiles para realizar copias de seguridad (backup) de un sistema ya instalado.</p>

<p>Lo normal es que un sistema Unix/Linux se arranque (boot) desde cinta cuando, tras producirse una falla (crash) del sistema, queremos restaurarlo a su situación anterior (proceso conocido como recovery system). </p>

<p>Un sistema Unix/Linux puede manejar uno o varios discos físicos, cada uno de los cuales puede contener uno o varios sistemas de archivos. Los sistemas de archivos son particiones lógicas del disco.</p>

<p>Hacer que un disco físico contenga varios sistemas de archivos permite una administración más segura, ya que si uno de los sistemas de archivos se daña, perdiéndose la información que hay en él, este accidente no se habrá propagado al resto de los sistemas de archivos que hay en el disco y podremos seguir trabajando con ellos para intentar una restauración o una reinstalación.</p>

<p>El kernel trabaja con el sistema de archivos a un nivel lógico y no trata directamente con los discos a nivel físico. Cada disco es considerado como un dispositivo lógico que tiene asociados unos números de dispositivos. Estos números se utilizan para indexar dentro de una tabla de funciones, la cual tenemos que emplear para acceder al manejador del disco. El manejador del disco se va a encargar de transformar las direcciones lógicas (kernel) de nuestro sistema de archivos a direcciones físicas del disco.</p>

<p>Un sistema de archivos se compone de una secuencia de bloques lógicos, cada uno de los cuales tiene un tamaño fijo. El tamaño de cada bloque es el mismo para todo el sistema de archivos y suele ser múltiplo de 512 bytes. Existen diferentes tipos de sistemas de archivos, cada uno de ellos ideado para una tarea específica y con un diferente tamaño de bloque.</p>

<p>El tamaño elegido para el bloque va a influir en las prestaciones globales del sistema. Por un lado nos interesa que los bloques sean grandes para que la velocidad de transferencia entre el disco y la memoria sea grande. Sin embargo, si los bloques lógicos son demasiados grandes, la capacidad de almacenamiento del disco se puede ver desaprovechada cuando abundan archivos pequeños que no llegan a ocupar un bloque completo. Valores típicos, en bytes, para el tamaño de un bloque son: 512, 1024, 2048.</p>

<p>La estructura de un sistema de archivos en Unix/Linux es:</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/imagentema1(2).png)

<p>En la figura podemos ver cuatro partes:</p>

<ul>
<li><n>El sector de arranque</n> (MBR) ocupa la parte del principio del sistema de archivos, típicamente el primer sector, y puede contener el código de arranque. Este código es un pequeño programa (GRUB, NTLOADER, LILO) que se encarga de buscar el sistema operativo y cargarlo en memoria para inicializarlo.</li>
<li></li>
<li></li>
<li></li>
</ul>

<p></p>

<p></p>

