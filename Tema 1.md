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
