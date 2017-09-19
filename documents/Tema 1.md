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

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/github1.png)

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

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1.png)

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

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(2).png)

<p>En la figura podemos ver cuatro partes:</p>

<ul>
<li><b>El sector de arranque</b> (MBR) ocupa la parte del principio del sistema de archivos, típicamente el primer sector, y puede contener el código de arranque. Este código es un pequeño programa (GRUB, NTLOADER, LILO) que se encarga de buscar el sistema operativo y cargarlo en memoria para inicializarlo.</li>
<li><b>El superbloque</b> describe el estado de un sistema de archivos. Contiene información acerca de su tamaño, total de archivos que puede contener, qué espacio queda libre, etc.</li>
<li><b>La lista de nodos índice (nodos-i)</b>. Se encuentra a continuación del superbloque. Esta lista tiene una entrada por cada archivo, donde se guarda una descripción del mismo: situación del archivo en el disco, propietario, permisos de acceso, fecha de actualización, etc. En algunas versiones de Unix/Linux es posible determinar el tamaño de la lista de nodos-i al momento de configurar el sistema.</li>
<li><b>Los bloques de datos</b> empiezan a continuación de la lista de nodos-i y ocupan el resto del sistema de archivos. En esta zona es donde se encuentra situado el contenido de los archivos a los que hace referencia la lista de nodos-i. Cada uno de los bloques destinados a datos sólo puede ser asignado a un archivo, tanto si lo ocupa completamente o no.</li>
</ul>

<h2>2.3 SUPERBLOQUE</h2>

<p>Como antes se mencionó, el superbloque contiene la descripción del estado del sistema de archivo. En el archivo cabecera <i><sys/filsys.h></i> está declarada una estructura en C que describe el significado del contenido del superbloque.</p>

<p>El superbloque contiene, entre otros elementos, la siguiente información:</p>

<ul>
<li>Tamaño del sistema de archivos.</li>
<li>Lista de bloques libres disponibles.</li>
<li>Índice del siguiente bloque libre en la lista de bloques libres.</li>
<li>Tamaño de la lista de nodos-i.</li>
<li>Total de nodos-i libres.</li>
<li>Lista de nodos-i libres.</li>
<li>Índice del siguiente nodo-i libre en la lista de nodos-i libres.</li>
<li>•	Campos de bloqueo de elementos de la lista de bloques y de nodos-i libres. Estos campos se emplean cuando se realiza una petición de bloque o de nodo-i libre.</li>
<li>•	Indicador que informa si el superbloque ha sido modificado o no.</li>
</ul>

<p>Cada vez que, desde un proceso, se accede a un archivo, es necesario consultar el superbloque y la lista de nodos-i. Como el acceso a disco suele degradar bastante el tiempo de ejecución de un programa, lo normal es que el kernel realice la E/S con el disco a través de un buffer cache y que el sistema tenga en memoria una copia del superbloque y de las listas de nodos.-i. Esto plantea un problema de consistencia de los datos, ya que una actualización en memoria del superbloque y de la tabla de nodos-i no implica una actualización inmediata en disco. La solución a este problema consiste en realizar periódicamente una actualización en disco de los datos de administración que mantenemos en memoria. De esta tarea se encarga un “daemon” que se arranca al inicializar el sistema. Naturalmente, antes de apagar el sistema hay que actualizar al superbloque y las tablas de nodos-i del disco. El programa shutdown se encarga de esta tarea y es fundamental tener presente que no se debe de realizar una salida del sistema sin haber invocado previamente a este programa, porque de lo contrario el sistema de archivos podría quedar seriamente dañado.</p>

<h2>2.4 NODOS-I (INODES)</h2>

<p>Cada archivo en un sistema Unix/Linux tiene asociado un nodo-i. El nodo-i contiene la información necesaria para que un proceso pueda acceder al archivo. Esta información incluye: propietario, derechos de acceso, tamaño, localización en el sistema de archivos, etc.</p>

<p>La lista de nodos-i se encuentra situada en los bloques que hay a continuación del superbloque. Durante el proceso de arranque del sistema, el núcleo lee la lista de nodos-i del disco y carga una copia en memoria, conocido como tabla de nodos-i. Las manipulaciones que haga el subsistema de archivos (parte del código del kernel) sobre los archivos van a involucrar a la tabla de nodos-i pero no la lista de nodos-i. Mediante este mecanismo se consigue una mayor velocidad de acceso a los archivos, ya que la tabla de nodos-i está cargada siempre en memoria. En el punto anterior vimos que existe un proceso del sistema (syncer) que se encarga de actualizar periódicamente el contenido de la lista de nodos-i con la tabla de nodos-i.</p>

<p>En el archivo cabecera <i><sys/ino.h></i> está declarada una estructura en C que describe la información de un nodo-i. Los campos que la integran son:</p>

<ul>
<li>Identificador del propietario del archivo. La posesión se divide entre un propietario individual y un grupo de propietarios y define el conjunto de usuarios que tiene derecho de acceso al archivo. El superusuario tiene derecho de acceso a todos los archivos del sistema.</li>
<li>Tipo de archivo. Los archivos pueden ser de datos, directorios, especiales de dispositivos (en modo carácter o en modo bloque) y tuberías (FIFO).</li>
<li>Tipo de acceso al archivo. El sistema protege los archivos estableciendo tres niveles de permisos: permisos del propietario, del grupo al que pertenece el propietario y del resto de los usuarios (conocidos también como el mundo). Cada clase de usuarios puede tener habilitados o deshabilitados los derechos de lectura, escritura y ejecución. Para los directorios el derecho de ejecución significa poder acceder o no a los archivos que contiene.</li>
<li>Registro de acceso al archivo. Dan información sobre la fecha de la última modificación del archivo, la última vez que se accedió a él y la última de vez que se modificaron los datos de su nodo-i.</li>
<li>Número de enlaces del archivo. Representa el total de los nombres que el archivo tiene en la jerarquía de directorios. Como veremos más adelante, un archivo puede tener asociados diferentes nombres que correspondan a diferentes rutas, pero a través de los cuales accedamos a un mismo nodo-i y, por consiguiente, a los mismos bloques de datos. </li>
<li>Entradas para los bloques de dirección de los datos de un archivo. Si bien los usuarios tratan los datos de un archivo como si fuesen una secuencia de byte contiguos, el kernel puede almacenarlos en bloques que no tiene por qué ser contiguos. En los bloques de dirección es donde se especifican los bloques de disco que contienen los datos del archivo.</li>
<li>Tamaño del archivo. Los bytes de un archivo se pueden direccionar indicando un desplazamiento a partir de la dirección de inicio del archivo (desplazamiento 0). El tamaño del archivo es igual al desplazamiento del byte más alto incrementado en uno. Por ejemplo, si un usuario crea un archivo y escribe en él sólo un byte en la posición 2000, el tamaño del archivo es 2001 bytes.</li>
</ul>

<p>Hay que hacer notar que el nombre del archivo no queda especificado en el nodo-i. Es en los archivos de tipo directorio donde a cada nombre del archivo se le asocia su nodo-i correspondiente.</p>

<p>También hay que señalar que hay una diferencia entre escribir el contenido de un nodo-i en disco y escribir el contenido de un archivo. El contenido del archivo (sus datos) cambia sólo cuando se escribe en él. El contenido de un nodo-i cambia cuando se modifican los datos del archivo o la situación administrativa del mismo (propietarios, permisos, enlaces, etc.).</p>

<p>La tabla de nodos-i contiene la misma información que la lista de nodos-i, además de lo siguiente:</p>

<ul>
<li>El estado del nodo-i, que indica:
  <ul>
  <li>Si el nodo-i está bloqueado,</li>
  <li>Si hay algún proceso esperando a que el nodo-i quede desbloqueado,</li>
  <li>Si la copia del nodo-i que hay en memoria difiere de la que hay en el disco,</li>
  <li>Si la copia de los datos del archivo que hay en memoria difiere de los datos que hay en el disco (caso de la escritura en el archivo a través del buffer caché).</li>
  </ul>
</li>
<li>El número de dispositivo lógico del sistema de archivos en donde se almacena.</li>
<li>El número de nodo-i. Como los nodos-i se almacenan en el disco en un arreglo lineal, al cargarlo en memoria, el kernel le asigna un número en función de su posición en el arreglo. El nodo-i del disco no necesita esta información.</li>
<li>Punteros a otros nodos-i cargados en memoria. El kernel enlaza los nodos-i sobre una cola de dispersión (cola hash) y sobre una lista libre. Las claves de acceso a la cola de dispersión nos las dan el número de dispositivo lógico del nodo-i y el número de nodo-i.</li>
<li>Un contador que indica el número de copias del nodo-i que están activas (por ejemplo, porque el archivo está abierto por varios procesos).</li>
</ul>

<h2>2.5 BLOQUES DE DATOS</h2>

<p>Los bloques de datos están situados a partir de la lista de nodos-i. Como se mencionó antes, cada nodo-i tiene unas entradas (bloques de direcciones) para localizar dónde están los datos de un archivo en el disco. Como cada bloque del disco tiene asociada una dirección, las entradas de direcciones consisten en un conjunto de direcciones de bloques del disco.</p>

<p>Si los datos de un archivo se almacenasen en bloques consecutivos, para poder acceder a ellos nos bastaría con conocer la dirección del bloque inicial y el tamaño del archivo. Sin embargo, esta estrategia provocaría un desaprovechamiento del disco, ya que tenderían a proliferar áreas libres demasiado pequeñas para ser usadas. Por ejemplo, supongamos que un usuario crea tres archivos, A, B y C, cada uno de los cuales ocupa 10 bloques en el disco. Supongamos que el sistema sitúa estos tres archivos en bloques contiguos. Si el usuario necesita añadir 5 bloques al archivo B, el sistema va a tener que buscar 15 bloques contiguos que se encuentren libres para copiar el archivo B en esa zona. Esto presenta dos inconvenientes, el primero es el tiempo que se pierde al copiar los 10 bloques de B que no cambiarían en contenido, el segundo es que los bloques que quedan libres sólo pueden contener archivos con un tamaño inferior o igual a 10 bloques. Esto va a provocar, a la larga, una microfragmentación del disco que lo va a dejar inservible.</p>

<p>El kernel puede minimizar la fragmentación del disco ejecutando periódicamente procesos para compactarlos, pero esto produce una degradación de las prestaciones del sistema en cuanto a velocidad. Por esto y para una mayor velocidad, el kernel reserva los bloques para un archivo de uno en uno, como se vayan requiriendo, y permite que los datos de un archivo estén esparcidos por todo el sistema de archivos.</p>

<p>Las entradas de direcciones de un nodo-i van a consistir en una lista de direcciones de los bloques que tienen los datos del archivo. Un cálculo simple nos muestra que almacenar la lista de bloques del archivo en un nodo-i es algo difícil de implementar. Por ejemplo, si los bloques son de 1 Kbyte y el archivo ocupa 10 Kbytes  (10 bloques), vamos a necesitar almacenar 10 direcciones en el nodo-i. Pero si el archivo es de 100 Kbytes (100 bloques), necesitaremos 100 direcciones para acceder a todos los datos. Así pues, esta estrategia nos impone que el tamaño del nodo-i debe ser variable, ya que si lo fijamos a un valor concreto, estamos fijando el tamaño máximo de los archivos que podemos manejar. Debido a lo poco manejable que resulta, la idea de nodos-i de tamaño variable es algo que no se implementa en ningún sistema.</p>

<p>Para conseguir que el tamaño de un nodo-i sea pequeño y a la vez podamos manejar archivos grandes, las entradas de direcciones de un nodo-i se ajustan al siguiente esquema:</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(3).png)

<p>Los nodos-i tienen 13 entradas. Las entradas marcadas como directas, en la figura, contienen direcciones de bloques en los que hay datos del archivo. La entrada marcada como indirecta simple direcciona un bloque de datos que contiene una tabla de direcciones de bloques de datos. Para acceder a los datos a través de una entrada indirecta, el kernel debe leer el bloque cuya dirección nos indica la entrada indirecta y buscar en él la dirección del bloque donde realmente está el dato, para a continuación leer ese bloque y acceder al dato. La entrada marcada como indirecta doble contiene la dirección de un bloque cuyas entradas actúan como entradas indirectas simples y la entrada indirecta triple direcciona un bloque cuyas entradas son indirectas dobles.</p>

<p>Este método puede extenderse para soportar entradas indirectas cuádruples, quíntuples, etc., pero en práctica tenemos más que suficiente con una indirecta triple.</p>

<p>Vamos a ver el caso práctico en el que los bloques son de 1 Kbyte y el bloque se direcciona con 32 bits (232 = 4 Gdirecciones posibles). En esta situación, un bloque de datos puede almacenar 256 direcciones de bloques y un archivo podría llegar a tener un tamaño de 16 Gbytes.</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(4).png)

<p>Los procesos acceden a los datos de un archivo indicando la posición, con respecto al inicio del archivo, del byte que queremos leer o escribir. El archivo es visto como una secuencia de bytes que empieza en el byte número 0 y llega hasta el byte cuya posición, con respecto al inicial, coincide con el tamaño del archivo menos uno. El kernel se encarga de transformar las posiciones de los bytes, tal y como las ve el usuario, a direcciones de los bloques de disco.</p>

<p>Veamos un ejemplo. Supongamos un archivo cuyos datos están en los bloques que nos indican las entradas de direcciones del nodo-i descrito en la siguiente figura:</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(5).png)

<p>Vamos a seguir suponiendo que el bloque tiene un tamaño de 1024 bytes. Si un proceso quiere acceder a un byte que se encuentra en la posición 9, 125 del archivo, el kernel calcula que ese byte está en el número 8 del archivo (empezando a numerar los bloques lógicos desde 0).</p>

<p>En efecto:</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(6).png)

<p>Para ver a qué bloque del disco corresponde el bloque lógico número 8 del archivo, hay que consultar el número de bloque almacenado en la entrada número 8 (entrada de dirección directa) de la tabla de direcciones del nodo-i. En el caso de la figura, el bloque de disco buscado es el 412. Dentro de este bloque, el byte 9,125 del archivo corresponde con el byte 933 con respecto al inicio del bloque (los bytes del bloque se numeran de 0 a 1023).</p>

<p>En efecto:</p>
![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(7).png)

<p>En este ejemplo el cálculo ha sido sencillo porque el byte buscado era accesible desde una entrada directa nodo-i.</p>

<p>Vamos a ver qué ocurre si queremos localizar en el disco el byte que se encuentra en la posición 425,000 del archivo. Si calculamos su bloque lógico de archivo, veremos que se encuentra en el bloque 415.</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(8).png)

<p>El total de bloques que podemos acceder con las entradas directas es de 10. Con la entrada indirecta simple podemos acceder a 256 direcciones, y con la indirecta doble, a 65,536. Por lo que el byte buscado estará en una entrada indirecta doble.</p>

<p>A esta entrada pertenecen los bloques comprendidos entre los números lógicos de archivo 266 y 65,581 (incluyéndolos) y el bloque buscado es el 415. Si nos fijamos en la figura, el número de bloque que contiene la entrada indirecta doble es el 12,456, que es el bloque de disco donde están las direcciones de los bloques con entradas indirectas simples.</p>

<p>La entrada número 0 del bloque indirecto doble nos da acceso a los bloques de archivo comprendidos entre el 266 y el 521, ya que cada entrada actúa como indirecto simple y da acceso a 256 bloques de datos. En la entrada 0 vemos que el número de bloque de disco del bloque indirecto simple que buscamos es 158. Dentro del bloque indirecto simple, la entrada que nos interesa es la diferencia entre 415 (bloque lógico del archivo) y 266 (bloque inicial al que da acceso el indirecto doble) que es 149. Según la figura, la entrada 149 del bloque de disco 158 contiene el número 9,126. Es en el bloque de disco 9,126 donde se encuentra el dato que buscamos, y en el byte 40 de este bloque está el byte 425,00 de nuestro archivo.</p>

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/imagentema1(9).png)

<p>Si observamos la figura anterior con más detenimiento, vemos que hay algunas entradas del nodo-i que están en 0. Esto significa que no referencian a ningún bloque del disco y que los bloques lógicos correspondientes del archivo no tienen datos. Esta situación se da cuando se crea un archivo y nadie escribe en los bytes correspondientes a estos bloques, por lo que permanecen en su valor inicial 0. Al no reservar el sistema bloques de discos para estos bloques lógicos, se consigue ahorro de los recursos del disco. Imaginemos que creamos un archivo y sólo escribimos un byte en la posición 1,048,276, esto significa que el archivo tiene un tamaño de 1 Mbyte. Si el sistema reservase bloques de discos para este archivo en función de su tamaño y no en función de los bloques lógicos que realmente tiene ocupados, nuestro archivo ocuparía 1024 bloques de discos en lugar de 1, como en realidad ocupa.</p>

<h1>3. USANDO EL GNU CC</h1>

<h2>3.1 CARACTERÍSTICAS DEL GNU CC</h2>

<p>gcc es uno de los compiladores mas usados en los ambientes de programa Unix/Linux. Es un compilador muy versátil y flexible. El compilador gcc nos permite:</p>

<ul>
<li>Detener el proceso de compilación en cualquier etapa y examinar la salida que nos da el compilador en cada una de ellas.</li>
<li>Es capaz de manejar varios dialectos de C, como ANSI C. Así como compilar programas en C++ y Objective C. </li>
<li>Se puede controlar la cantidad y tipo de información de “debugging” que estará embebida en el archivo binario resultante.</li>
<li>Realizar optimización del código generado.</li>
</ul>

<h2>3.2 UN BREVE TUTORIAL</h2>

<p>Antes de ver a profundidad al gcc, veremos un pequeño ejemplo que nos ayudará para empezar a usarlo. Hagamos un “hola mundo”.</p>

```c
#include <stdio.h>

int main () {
  fprintf (stdout, "Hola Mundo!!!\n");
}
```
<p>Para compilar y correr este programa, debemos ejecutar las siguientes instrucciones:</p>

```
$ gcc programa1.c -o programa
$ ./programa1
$Hola Mundo!!!
$
```

<p></p>

