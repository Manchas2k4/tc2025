# 7. Señales y Funciones de Tiempo
## 7.1 Concepto de Señal
Las señales son interrupciones software que pueden ser enviadas a un proceso para informarle de algún evento asíncrono o situación especial. El término señal se emplea también para referirse a un evento.

Los procesos pueden enviarse señales unos a otros a través de la llamada `kill` y es bastante frecuente que, durante la ejecución, un proceso reciba señales procedentes del kernel. Cuando un proceso recibe una señal, pueden tratarla de tres formas diferentes:
1. Ignorar la señal, con lo cual es inmune a la misma.
2.	Invocar a la rutina de tratamiento por defecto. Esta rutina no la codifica el programa, sino que la aporta el kernel. Según el tipo de señal, la rutina de tratamiento por defecto va a realizar una acción u otra. Por lo general suele provocar la terminación del proceso mediante una llamada a exit. Algunas señales no sólo provocan la terminación del proceso, sino que además  hacen que el kernel genere, en el directorio de trabajo actual del proceso, un archivo llamado core que contiene un volcado de memoria del contexto del proceso. Este archivo podrá ser examinado con ayuda de un programa depurador (`adbm`, `sdb`) para determinar qué señal provocó la terminación del proceso y en qué punto exacto de su ejecución se produjo. Este mecanismo es útil a la hora de deputar programas que contienen errores de manejo de los números de punto flotante, instrucciones ilegales, acceso a direcciones fuera de rango, etc.
3.	Invocar a una rutina propia que se encarga de tratar la señal. Esta rutina es invocada por el kernel en el supuesto de que esté montada y será responsabilidad del programa codificarla para que tome las acciones pertinentes como tratamiento de la señal. En estos casos, el programa no va a terminar a menos que la rutina de tratamiento indique lo contrario.

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/Tarea%207.jpg "Tarea 7 Foto 1")

En la figura anterior vemos esquematizada la evolución temporal de un proceso y cómo, a lo largo de su ejecución, recibe las señales procedentes del kernel. La primera señal que recibe no provoca que el proceso cambie el curso de su ejecución, esto es debido a que la acción que está activada es que el proceso ignore la señal. El proceso prosigue su ejecución y recibe una segunda señal que lo fuerza a entrar en una rutina de tratamiento. Esta rutina, después de tratar la señal, puede optar por tres acciones: restaurar la ejecución del proceso al punto donde se produjo la interrupción, finalizar el proceso o restaurar alguno de los estados pasados del proceso y continuar la ejecución desde ese punto (más adelante veremos que esto se consigue con las funciones estándar `sigsetjmp` y `siglongjmp`). El proceso puede también recibir una señal que lo fuerce a entrar en la rutina de tratamiento por defecto.

En las siguientes secciones volveremos a hablar sobre estos conceptos y veremos cómo se especifica cuál de las tres formas es la elegida para tratar una señal.
## 7.2 Tipos de Señales
Cada señal tiene asociado un número entero positivo, que es intercambiado cuando algún proceso envía una señal a otro. En UNIX System V hay definidas 19 señales, 4.3BSD, Mac y Linux tienen 31, mientras que Solaris soporta 38.  Las señales están prácticamente en todas las versiones de UNIX, y a éstas cada fabricante le añade las que considera necesarias. Podemos clasificar las señales en los siguientes grupos:
- Señales relacionadas con la terminación de procesos.
-	Señales relacionadas con las excepciones inducidas por los procesos. Por ejemplo, el intento de acceder fuera del espacio de direcciones virtuales, los errores producidos al manejar número de punto flotante, etc.
-	Señales relacionadas con los errores irrecuperables originados en el transcurso de una llamada al sistema.
-	Señales originadas desde un proceso que se está ejecutando en modo usuario. Por ejemplo, cuando un proceso envía una señal a otro vía kill, cuando un proceso activa un temporizador y se queda en espera de la señal de alarma, etc.
-	Señales relacionadas con la interacción con la terminal. Por ejemplo, pulsar la tecla break.
-	Señales para ejecutar un proceso paso a paso. Son usadas por los depuradores.
En el archivo de cabecera `<signal.h>` están definidas las señales que pueden ser manejar por el sistema y sus nombres. 

**señal**|**significado**
---|---
SIGHUP|Desconexión. Es enviada cuando una terminal se desconecta de todo proceso del que es terminal de control. También se envía a todos los procesos de un grupo cuando el líder del grupo termina su ejecución. La acción por defecto de esta señal es terminar la ejecución del proceso que la recibe.
**SIGINT**|Interrupción. Se envía a  todo proceso asociado con una terminal de control cuando se pulsa la tecla de interrupción (`CTRL + C`). Su acción por defecto es terminar la ejecución del proceso que la recibe.
**SIGQUIT**|Salir. Similar a SIGINT, pero es generada al pulsar la tecla de salida (`CTRL + \`). Su acción por defecto es genera un archivo core y terminar el proceso.
**SIGILL**|Instrucción ilegal. Es enviada cuando el hardware detecta una instrucción ilegal. En los programa escritos en C/C++ suele producirse este tipo de errores cuando manejamos apuntadores a funciones que no han sido correctamente inicializados. Su acción por defecto es general un archivo core y terminar el proceso.
**SIGTRAP**|`Trace trap`. Es enviada después de ejecutar cada instrucción, cuando el proceso se está ejecutando paso a paso. Su acción por defecto es genera un archivo `core` y terminar el proceso. Es utilizado cuando se utiliza un depurador.
**SIGIOT**|`I/O trap instruction`. Se envía cuando se da un fallo de hardware. La naturaleza de este fallo depende de la computadora. Es enviada cuando llamamos a la función abort, que provoca el suicidio del procesos generando un archivo `core`.
**SIGEMT**|`Emulator trap instruction.` También indica un fallo de hardware. Raras veces se utiliza. Su acción por defecto es generar un archivo core y terminar el proceso.
**SIGFPE**|Error de punto flotante. Es enviada cuando el hardware detecta un error de punto flotante, como el uso de un número de punto flotante con un forma desconocido, errores de desbordamiento, etc. Su acción por defecto es generar un archivo `core` y terminar el proceso.
**SIGKILL**|Terminación abrupta. Esta señal provoca irremediablemente la terminación del proceso. No puede ser ignorada y siempre que se recibe se ejecuta su acción por defecto, que consiste en generar un archivo `core` y terminar el proceso.
**SIGBUS**|Error de bus. Se produce cuando se da un error de acceso a memoria. Las dos situaciones típicas que la provocan suelen ser intentar acceder a una dirección que físicamente no existe o intentar acceder a una dirección impar, violando así las reglas de alineación que impone el hardware. Su acción por defecto es generar un archivo `core` y terminar el proceso.
**SIGSEGV**|Violación de segmento. Es enviada a un proceso cuando intenta acceder a datos que se encuentran fuera de su segmento de datos. Su acción por defecto es generar un archivo `core` y terminar el proceso.
**SIGSYS**|Argumento erróneo en una llamada al sistema. No se usa.
**SIGPIPE**|Intento de escritura en un pipe del que no hay nadie leyendo. Esto suele ocurrir cuando el proceso de lectura termina de una forma anormal. Su acción por defecto es generar un archivo `core` y terminar el proceso.
**SIGALRM**|Despertador. Es enviada a un proceso cuando alguno de sus temporizadores descendentes llega a cero. Su acción por defecto es terminar el proceso. Se basa en el tiempo real.
**SIGTERM**|Finalización controlada. Es la señal utilizada para indicarle a un proceso que debe terminar su ejecución. Esta señal no es tajante como `SIGKILL` y puede ser ignorada. Lo correcto es que la rutina de tratamiento de esta señal se encargue de tomar las acciones necesarias antes de terminar un proceso (como, por ejemplo, borrar los archivos temporales) y llame a la rutina `exit`. Esta señal es enviada a todos los procesos cuando se emite una orden `shutdown`. Su acción por defecto es terminar el proceso.
**SIGUSR1**|Señal número 1 de usuario. Esta señal está reservada para uso del programador. Ninguna aplicación estándar va a utilizarla y su significado es el que le quiera dar el programador en su aplicación. Su acción por defecto es terminar el proceso.
**SIGUSR2**|Señal número 2 de usuario. Su significado es idéntico al de SIGUSR1.
**SIGCLD/SIGCHLD**|Terminación del proceso hijo. Es enviada al proceso padre cuando alguno de sus procesos hijos termina. Esta señal es ignorada por defecto.
**SIGPWR**|Fallo de alimentación. Esta señal tiene diferentes interpretaciones. En algunos sistemas es enviada cuando se detecta un fallo de alimentación y le indica al proceso que dispone tan sólo de unos instantes de tiempo antes de que se produzca una caída del sistema. En otros sistemas, esta señal es enviada, después de recuperarse de un fallo de alimentación, a todos aquellos procesos que estaban en ejecución y que se han podido rearrancar. En estos casos, los procesos deben disponer de mecanismos para restaurar las posibles pérdidas producidas durante la caída de la alimentación.
**SIGVTALRM**|Despertador. Es enviada a un proceso cuando alguno de sus temporizadores descendentes llega a cero. Su acción por defecto es terminar el proceso. Se basa en el tiempo que el proceso está en modo usuario. Se puede usar junto con `SIGPROF` para medir el tiempo usado por el proceso en modo usuario y kernel.
**SIGPROF**|Despertador. Es enviada a un proceso cuando alguno de sus temporizadores descendentes llega a cero. Su acción por defecto es terminar el proceso. Se basa en el tiempo que el proceso está en modo kernel. Se puede usar junto con `SIGVTALRM` para medir el tiempo usado por el proceso en modo usuario y kernel.
**SIGIO/SIGPOLL**|Señal de entrada/salida asíncrona. Cuando se realiza la operación `I_SETSIG` sobre un descriptor de archivos a través de la llamada `ioctl`, el kernel envía esta señal cuando un evento ocurre sobre el descriptor, por ejemplo cuando existen datos que leer/escribir. Indica que un dispositivo o archivo está listo para una operación de entrada/salida. Su acción por defecto es ignorar la señal.
**SIGWINCH**|Cambio del tamaño de una ventana. Se usa en interfaces gráficas orientadas a ventanas como X-WINDOWS. Su acción por defecto es ignorar la señal.
**SIGSTOP**|Señal de pausa de un proceso. Esta señal no proviene de una terminal de control. La señal no puede ser ignorada, ni capturada y su acción por defecto es pausar el proceso. El proceso solo podrá continuar después de haber recibido la señal de `SIGCONT`.
**SIGTSTP**|Señal de pausa procedente de una terminal. Es generado por el teclado (tecla `SUSP` o `CTRL – Z`).  El proceso solo podrá continuar después de haber recibido la señal de `SIGCONT`. A diferencia de `SIGSTOP` esta señal puede ser ignorada o manejada. Su acción por defecto es ignorar la señal.
**SIGCONT**|Continuar. Señal para reanudar la ejecución de un proceso. Su acción por defecto es ignorar la señal.
**SIGTTIN**|La reciben los procesos que se ejecutan en segundo plano y que intentan leer datos de una terminal de control. Su acción por defecto es parar el proceso.
**SIGTTOU**|La reciben los procesos que se ejecutan en segundo plano y que intentan escribir en una terminal de proceso. Su acción por defecto es parar el proceso.
**SIGURG**|Indica que ha llegado un dato urgente a través de un canal de entrada/salida asíncrono. Un ejemplo sería cuando hay datos que leer de un descriptor de archivo que está conectado a un socket. Su acción por defecto es ignorar la señal.
**SIGXCPU**|Le indica al proceso que la recibe que ha superado su tiempo de CPU asignado. Su acción por defecto es ignorar la señal.
**SIGXFSZ**|La indica al proceso que la recibe que superado el tamaño máximo de archivo que puede manejar. Su acción por defecto es terminar el proceso.
## 7.3 Manejo de Señales
Ahora estudiaremos el manejo de señales que brinda UNIX.
### 7.3.1 Envío de señales – kill y raise
Para enviar una señal desde un proceso a otro o a un grupo de procesos, emplearemos la llamada `kill`. Su declaración es:

`#include <signal.h>
int kill(pid_t pid, int sig);`

`pid` identifica el conjunto de procesos al que queremos enviarle la señal. `pid` es un número entero y los distintos valores que puede tomar tienen los siguientes valores.

**PID**|**Significado**
---|---
**pid > 0**|Es el `PID` del proceso al que le enviamos la señal.
**pid = 0**|La señal es enviada a todos los procesos que pertenecen al mismo grupo que el proceso que la envía.
**pid = -1**|La señal es enviada a todos aquellos procesos cuyo identificador real es igual al identificador efectivo del proceso que la envía. Si el proceso que la envía tiene identificador efectivo de superusuario, la señal es enviada a todos los procesos, excepto al proceso 0 (`swapper`) y al proceso 1 (`init`).
**pid < -1**|La señal es enviada a todos los procesos cuyo identificador de grupo coincide con el valor absoluto de `pid`.



