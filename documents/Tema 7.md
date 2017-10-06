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

En todos los casos, si el identificador efectivo del proceso no es el del superusuario o si el proceso que envía la señal no tiene privilegios sobre el proceso que la va a recibir, la llamada a `kill` falla.

`sig` es el número de la señal que queremos enviar. Si `sig` vale 0 (señal nula), se efectúa una comprobación de errores, pero no se envía ninguna señal. Esta opción se puede utilizar para verificar la validez del identificador `pid`.

Si el envío se realiza satisfactoriamente, `kill` devuelve 0; en caso contrario, devuelve -1 y en `errno` estará el código del proceso producido.
En el siguiente ejemplo vemos cómo un proceso envía una señal a su proceso hijo para forzar su terminación.
`#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
	int pid;
	
	if ((pid = fork()) == 0) {
		while (1) {
			fprintf(stdout, "HIJO. PID = %d\n", getpid());
			sleep(1);
		}
	}
	sleep(10);
	fprintf(stdout, "PADRE. Terminacion del proceso %d\n", pid);
	kill(pid, SIGTERM);
	exit(0);
}`

Si queremos que un proceso se envíe señales a sí mismo, podemos usar la llamada `raise`. Su declaración es:
```
#include <signal.h>
int raise(int sig);
```

`sig` es el número de la señal que queremos enviar. `raise` se puede codificar a partir de `kill` de la siguiente manera:
```
int raise(int sig) {
return kill(getpid(), sig);
}
```
### 7.3.2 Tratamiento de Señales

Para especificar qué tratamiento debe realizar un proceso al recibir una señal, se emplea la función `signal`. Su declaración es la siguiente:
```
#include <signal.h>
void (*signal (int sig, void (*action) (int))) (int);
```
`signal` tiene una declaración que puede resultarnos extraña, pero es frecuente en los programas C/C++. Como podemos ver, `signal` es del tipo “función que devuelve un apuntador a una función `void` que recibe parámetros”.

`sig` es el número de la señal sobre la que queremos especificar la forma de tratamiento.

`action` es la acción que queremos que se inicie cuando se reciba la señal. action puede tomar tres tipos de valores:

**Valores**|**Significado**
---|---
**SIG_DFL**|Indica que la acción a realizar cuando se recibe la señal es la acción por defecto asociada a la señal (manejador por defecto). Por lo general, esta acción es terminar el proceso y en algunos casos también incluye generar el archivo `core`.
**SIG_IGN**|Indica que la señal se debe ignorar.
**dirección**|Es la dirección de la rutina de tratamiento de la señal (manejador suministrado por el usuario). La declaración de esta función debe ajustarse el siguiente modelo.```#include <signal.h> void handler(sig [, code, scp]) int sig, code; struct sigcontext *scp;```

Cuando se recibe la señal `sig`, el kernel es quien se encarga de llamar a la rutina `handler` pasándole los parámetros `sig`, `code` y` scp`. `sig` es el número de la señal, `code` es una palabra que contiene la información sobre el estado del hardware en el momento de invocar a `handler` y `scp` contiene información de contexto definida en `<signal.h>`. Tanto `code` como `scp` son parámetros opcionales y dependientes de la arquitectura de la computadora

La llamada a la rutina `handler` es asíncrona, lo cual quiere decir que puede darse en cualquier instante de la ejecución del programa. Esta rutina debe estar codificada para tratar las situaciones especiales que ocasionan que se produzca el envío de las señales.

La llamada a `signal` devuelve el valor que tenía `action`, que puede servirnos para restaurarlo en cualquier instante posterior. Si se produce algún error, `signal` devuelve SIG_ERR y en `errno` estará el código del error producido.

Los valores del `SIG_DFL, SIG_IGN` y `SIG_ERR` son direcciones de funciones ya que los debe poder devolver `signal`. Sin embargo deben ser direcciones que sepamos que nuca van a estar ocupadas por otras funciones. Para darle solución a este problema, las constantes anteriores se definen de la siguiente forma:
```
#define SIG_DFL ((void (*) ()) 0)
#define SIG_IGN ((void (*) ()) 1)
#define SIG_ERR ((void (*) ()) -1)
```
La conversión explícita de tipo que aparece delante de las constantes -1, 1 y 0 fuerza a que estas constantes sean tratadas como direcciones de inicio de funciones. Estas direcciones no van a contener ninguna función, ya que en todas las arquitecturas UNIX son zona reservada para el kernel. Además, la dirección -1 ni siquiera tiene existencia física.

Como primer ejemplo de tratamiento de señales, vamos a codificar un programa que trata la señal de `SIGINT`, generada al pulsar las teclas `CTRL+C` (interrupción).
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigint_handler(int sig) {
	fprintf(stdout, "Señal número %d.\n", sig);
}

int main(int argc, char *argv[]) {	
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		perror("signal");
		exit(-1);
	} 
	while (1) {
		fprintf(stdout, "En espera de CTRL+C\n");
		sleep(30);
	}
exit(0);
}
```
### 7.3.3 Señales Inestables

En las primeras versiones del UNIX, las señales no eran confiables. Esto quiere decir, que la señales podían perderse: sí una señal ocurría, el proceso quizás no podría enterarse nunca. También, un proceso tenía poco control sobre una señal: solo podía cacharla o ignorarla,  no es posible bloquearla.
Un problema que ocurría en las primeras versiones es que la acción para una señal era restaurada a la acción por defecto que tenía asignada la señal. El siguiente es un ejemplo clásico de un código utiliza para manejar una señal. 
```
void sigint_handler(int sig) {
	…
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		…
	}
}

int main(int argc, char *argv[]) {	
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		…
	} 
}
```
El problema con este fragmento de código es que existe una ventana de tiempo después de que la señal ha ocurrido y se realiza la siguiente llamada a `signal`. Durante este tiempo puede ocurrir una segunda señal. Esta segunda señal en vez de utilizar el manejador asignado, realizaría la acción por defecto.

Otro problema con las versiones anteriores es que el proceso no es capaz de apagar una señal cuando no queremos que esa señal no suceda. Lo que se podía hacer era ignorar la señal. Pero existen ocasiones en que nosotros le queremos decir al sistema: “previene que las siguientes señales ocurran, pero recuérdame si suceden”. El clásico ejemplo que demuestra esta falla está indicado en el segmento de código que captura una señal y establece una bandera que indica que la señal ha ocurrido:
```
int sig_int_flag = 0;

void sigint_handler(int sig) {
	…
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		…
	}
sig_int_flag ) 1;
}

int main(int argc, char *argv[]) {	
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		…
	} 
	while (sig_int_flag == 0) {
		pause();
	}
}
```
Aquí, el proceso utiliza la llamada `pause` para dormirse hasta que una señal es capturada. Cuando la señal es capturada, el manejador establece la bandera a un valor diferente de cero. El proceso es automáticamente despertado por el kernel después de que el manejador termina, nota que la bandera es diferente de cero y hacer todo lo que tiene que hacer. Pero, existe una ventana de tiempo donde todo puede ir mal. Si la señal ocurre después de verificar el valor de la bandera, pero antes de la llamada a `pause`, el proceso podría dormir por siempre (asimiento que la señal nueva es generara nuevamente), ya que esta ocurrencia de la señal es ignorada. 

### 7.3.4 En Espera de Señales – Pause
En ocasiones puede interesar que un proceso suspenda su ejecución en espera de que ocurra algún evento exterior a él. Por ejemplo, al ejecutar una entrada/salida. Para estas situaciones nos valemos de la llamada a pause, cuya declaración es la siguiente:
```
#include <unistd.h>
pause();
```
`pause` hace que el proceso quede en espera de la llegada de alguna señal. Cuando esto ocurre, y después de ejecutarse la rutina de tratamiento de la señal, pause devuelve el valor -1 y en errno sitúa el valor `EINTR` que significa que se ha producido una interrupción de la llamada. En otras llamadas al sistema, ésta es una condición de error, pero en el caso de pause es su forma correcta de operar. El programa continúa con la sentencia que sigue a pause.

El programa siguiente es un ejemplo en el que un proceso está esperando una señal y cuando recibe la señal `SIGUSR1` presenta en pantalla un número aleatorio.
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigterm_handler(int sig) {
	fprintf(stdout, "Terminación del proceso %d a petición del usuario.\n", getpid());
	exit(-1);
}

void sigusr1_handler(int sig) {
	signal(sig, SIG_IGN);
	fprintf(stdout, "%d\n", rand());
	signal(sig, sigusr1_handler);
}

int main(int argc, char * argv[]) {
	fprintf(stdout, "PID = %d\n", getpid());
	signal(SIGTERM, sigterm_handler);
	signal(SIGUSR1, sigusr1_handler);
	while (1) {
		pause();
	}
exit(0);
}
```
Para ejecutar este programa y que muestre los números aleatorios, debemos enviarle la señal `USR1` con la orden `kill`,

`$ kill –s USR1 pid`

Podemos terminar la ejecución del proceso enviándole la señal `SIGTERM`.

`$ kill –s TERM pid`

### 7.3.5 Saltos Globales – SIGsetjmp y SIGlongjmp
En párrafos anteriores hemos indicado que la rutina de tratamiento de una señal puede hacer que el proceso vuelve a alguno de los estados por los que ha pasado con anterioridad. Esto no sólo es aplicable a las rutinas de tratamiento de señales sino que se puede extender a cualquier función. Para realizar esto nos valemos de las funciones estándar `sigsetjmp` y `siglongjmp`. Sus declaraciones las podemos ver a continuación:
```
#include <setjmp.h>
int sigsetjmp (jmp_buf env, int savemask);
void siglongjmp (jmp_buf env, int val);
```
Si `savemask` es 0,  `sigsetjmp` guarda el entorno de pila en `env` para un uso posterior de `siglongjmp`. `sigsetjmp` devuelve el valor 0 en su primera llamada. El tipo de `env`, `jmp_buf`, está definido en el archivo cabecera `<setjmp.h>`.

`siglongjmp` restaura el entorno guardado en env por una llamada previa a `sigsetjmp`. Después de haberse ejecuta la llamada a `siglongjmp`, el flujo de la ejecución del programa vuelve al punto donde se hizo la llamada a `sigsetjmp`. Pero en este caso `sigsetjmp` devuelve el valor val que hemos pasado mediante `siglongjmp`. Esta es la forma de averiguar si `sigsetjmp` está saliendo de una llamada para guardar el entorno o de una llamada `siglongjmp`. `siglongjmp` no puede hacer que `sigsetjmp` devuelva 0, ya que en el caso de que val sea igual a 0, `sigsetjmp` va a regresa 1.

En la siguiente figura podemos ver representa la forma de trabajar de `sigsetjmp` y `siglongjmp`.

Las funciones `sigsetjmp` y `siglongjmp` se puede ver como una forma elaborada de implementa una sentencia `goto` capaz de saltar desde una función a etiquetas que están en la misma o en otra función. Las etiquetas serían los entornos guardados por `sigsetjmp` en la variable `env`.

Como ejemplo, vamos a ver un programa que cuenta desde 1 hasta 10 incrementando su valor cada 10 segundos. Además, cada 10 segundos se va a establecer un punto de retorno de tal forma que si se recibe la señal `SIGUSR1` en algún momento, el programa va a reiniciar su ejecución en ese punto.
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf env;

void sigusr1_handler(int sig) {
	//signal(SIGUSR1, sigusr1_handler);
	siglongjmp(env, 1);
}

int main(int argc, char * argv[]) {
	int i;
	
	fprintf(stdout, "PID = %d\n", getpid());
	signal(SIGUSR1, sigusr1_handler);
	for (i = 0; i < 10; i++) {
		if (sigsetjmp(env, 1) == 0) {
			fprintf(stdout, "Punto de retorno en el estado %d.\n", i);
		} else {
			fprintf(stdout, "Regresando al punto de retorno en el estado %d.\n", i);
		}
		sleep(10);
	}
}
```
### 7.3.6 Función sigaction
La función `sigaction` nos permite examinar o modificar (o ambas) la acción asociada con una señal en particular. Esta función reemplaza a la llamada `signal` de las primeras versiones de UNIX. De hecho, es posible implementar `signal` usando `sigaction`.
Su declaración es la siguiente:
```
#include <signal.h>
int sigaction(int signo, const struct sigaction *restrict act, 
struct signaction *restrict oact);
```
`signo` es el número de señal cuya acción que queremos analizar o modificar. `act` es un apuntador a una estructura del tipo `sigaction`. Si el valor de `act` es diferente de nulo significa que queremos modificar la acción y el kernel nos regresa la acción previa a través del apuntador `oact`. La definición de la estructura `sigaction` es la siguiente:
```
struct sigaction {
	void (*sa_handler) (int); 	/* Dirección del manejador */
						/* o SIG_IGN, o SIG_DFL */ 
	sigset_t sa_mask;		/* Señales adicionales a bloquear. */
	int sa_flags;		/* Opciones adicionales. */
	void (*sa_sigaction) (int, siginfo_t *, void *); 
/* manejador alternativo. */
};
```
El campo `sa_handler` es un apuntador a una función que devuelve tipo `void` y tiene el mismo significado que el parámetro `action` de la llamada `signal`. Este campo se utiliza para indicar cuál va a ser la rutina de tratamiento de la señal. Al igual que en el caso de `signal`, puede tomar tres valores con diferente significado:

**Valores**|**Significado**
---|---
**SIG_DFL**|La rutina de tratamiento va a ser la rutina de tratamiento por defecto.
**SIG_IGN**|Indica que la señal se debe ignorar.
**dirección**|La rutina de tratamiento empieza en la dirección indicada y ha sido codificada por el usuario.

El campo `sa_mask` codifica, en cada uno de sus bits, las señales que no deseamos que han tratadas si sin recibidas mientras se está ejecutando la rutina de tratamiento actual. Normalmente, este campo está en 0, lo que indica que mientras está tratando una señal, cualquier otra señal puede interrumpir.  Si alguno de los bits de `sa_mask` está en 1, vamos a impedir el anidamiento cuando se recibe esa señal. Cuando el manejador termina, la máscara de señales del proceso es regresado a su estado previo.
El campo `sa_flags` codifica cuál va a ser la semántica (significado) que se emplee en la recepción de la señal. Los siguientes bits están definidos para este campo:

**Valores**|**Significado
---|---
**SA_INTERRUPT**|Las llamadas a sistema que son interrumpidos por esta señal no son automáticamente reiniciadas.
**SA_NOCLDSTOP**|Si signo es `SIGCHLD`, no genera esta señal cuando un proceso hijo se detiene por un control de trabajo (`SIGSTOP`, `SIGTSTP`, `SIGTTIN`, `SIGTTOU`). Esta señal, sin embargo, es generada cuando el hijo termina.
**SA_NOCLDWAIT**|Si signo es `SIGCHLD`, esta opción previene al sistema de crear procesos zombis. El proceso que ejecuta la llamada se bloquea hasta que todos sus hijos hayan terminado y entonces regresa 1 y `errno` es igual a `ECHILD`.
**SA_NODEFER**|Cuando esta señal es capturada, la señal no es automáticamente bloqueada por el sistema mientras el manejador se ejecuta (a menos que la señal esté incluida en `sa_mask`). Este comportamiento es el equivalente al de las primeras versiones de **UNIX**.
**SA_ONSTACK**|Si una pila alterna ha sido declarada con `sigaltstack`, la señal es entregada al proceso en esa pila alterna.
**SA_RESETHAND**|Esto hace que el manejador de la señal sea nuevamente el manejador por defecto (`SIG_DFL`).
**SA_RESTART**|Las llamadas a sistema que son interrumpidas por esta señal son automáticamente reiniciadas.
**SA_SIGINFO**|Esta opción provee información adicional al manejador: aun apuntador a una estructura `siginfo_t` y un apuntador a un identificador del contexto del proceso.

El campo `sa_sigaction` es un manejador alterno de la señal usado cuando `SA_SIGINFO` es utilizado con la llamada a `sigaction`. Normalmente, el manejador de la señal es:
`void handler(int signo);`
Pero si SA_SIGINFO es usado, la definición del navegador debe ser:
`void handler(int signo, siginfo_t *info, void *context);`
La estructura `siginfo_t` contiene información acerca del porque la señal fue generada.
```
struct siginfo_t {
    int      si_signo;  /* Número de la señal */
    int      si_errno;  /* Número de error */
    int      si_code;   /* Código de la señal */
    pid_t    si_pid;    /* ID del proceso que envía la señal */
    uid_t    si_uid;    /* ID real del proceso que envía la señal */
    int      si_status; /* valor de salida o señal */
    clock_t  si_utime;  /* Tiempo de usuario consumido */
    clock_t  si_stime;  /* Tiempo de sistema consumido */
    sigval_t si_value;  /* Valor de la señal */
    int      si_int;    /* Señal POSIX.1b */
    void *   si_ptr;    /* Señal POSIX.1b */
    void *   si_addr;   /* Localidad de memoria que ha causado 
			    la señal */
    int      si_band;   /* Número de banda del evento */
    int      si_fd;     /* Descriptor del archivo */
};
```
En el siguiente programa veremos las tres formas de tratamiento de una señal: con un manejador, ignorando la señal o con la rutina por defecto. Este programa se envía a sí mismo en repetidas ocasiones la señal `SIGUSR1`; según  la rutina de tratamiento que esté instalada, la respuesta será una o otra.
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigusr1_handler(int sig) {
	fprintf(stdout, "sigusr1_handler: señal recibida: %d.\n", sig);
}

int main(int argc, char *argv[]) {
	struct sigaction action;
	
	action.sa_handler = sigusr1_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGUSR1, &action, 0);
	fprintf(stdout, "Envio de señal de SIGUSR1. Manejador activo.\n");
	kill(getpid(), SIGUSR1);
	fprintf(stdout, "Envio de señal de SIGUSR1. Manejador activo.\n");
	kill(getpid(), SIGUSR1);
	
	action.sa_handler = SIG_IGN;
	sigaction(SIGUSR1, &action, 0);
	fprintf(stdout, "Ignorar activo.\n");
	kill(getpid(), SIGUSR1);
	fprintf(stdout, "Ignorar activo.\n");
	kill(getpid(), SIGUSR1);
	
	action.sa_handler = SIG_DFL;
	sigaction(SIGUSR1, &action, 0);
	fprintf(stdout, "Rutina de tratamiento por defecto activo.\n");
	kill(getpid(), SIGUSR1);
	
	fprintf(stdout, "Este mensaje no aparece en pantalla.\n");
	exit(0);
}
```
Otro ejemplo que hacer con `sigaction` es la implementación de la función `signal`:
typedef void function(int);
```
function* signalX(int sig, function *fun) {
	struct sigaction act, oact;
	
	act.sa_handler = fun;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sig == SIGALRM) {
		#ifndef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
		#endif
	} else {
		#ifndef SA_RESTART
		act.sa_flags |= SA_RESTART;
		#endif
	}
	if (sigaction(sig, &act, &oact) < 0) {
		return (SIG_ERR);
	}
	return oact.sa_handler;
}
```
### 7.3.7 Funciones para manejar conjuntos de señales

Existe un tipo de dato (`sigset_t`) que nos permite manejar un conjunto de señales. Con este tipo de dato usaremos algunas funcionas como `sigprocmask` para decirle al kernel que no permita que ninguna de las  señales incluidas en este conjunto ocurran. Para manipular estos conjuntos de señales tenemos 5 funciones:
```
#include <signal.h>
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int sig);
int sigdelset(sigset_t *set, int sig);
int sigismember(const sigset_t *set, int sig);
```
La función `sigemptyset` inicializa a `set` con un valor que indica que todas las señales son excluidas. La función `sigfillset` inicializa a `set` con un valor que indica que todas las señales van a ser incluidas. 

Una vez que hemos inicializado el conjunto de señales, podemos agregar o eliminar señales de ese grupo. La función `sigaddset` nos permite agregar una señal al conjunto, mientras que `sigdelset` nos permite eliminar una señal del conjunto.
Por último, `sigismember` nos permite determinar si una señal específica se encuentra defina dentro del conjunto.

### 7.3.8 Función sigprocmask

Cada proceso tiene una máscara de señales que define el conjunto de señales que actualmente no se están entregando al proceso (bloqueadas). Un proceso puede examinar y/o cambiar esta máscara a través de la llamada a `sigprocmask`. Su definición es la siguiente:
```
#include <signal.h>
int sigprocmask(int how, const sigset_t *restrict set, 
			sigset_t *restrict oset);
```
Primero, si oset es un apuntador no nulo, la función regresa la máscara actual del proceso a través de `oset`.
Segundo, si `set` es un apuntador no nulo, el argumento how indica cómo es que la máscara actual será modificada. Los posibles valores que puede tomar how son:

**Valores**|**Significado**
---|---
**SIG_BLOCK**|La nueva máscara del proceso es la unión entre la máscara actual y el conjunto indicado por set. Es decir que `set` tiene señales adicionar que queremos bloquear.
**SIG_UNBLOCK**|La nueva máscara para el proceso es la intersección de la máscara actual y el conjunto indicado por set. Es decir que `set` tiene señales que queremos desbloquear.
**SIG_SETMASK**|La nueva señal es la que está siendo indicar por `set`.

Si set es un apuntador nulo, la máscara de señales no es cambiada y `how` es ignorado. 

Si cuando se ejecuta la llamada a `sigprocmask` existe una señal pendiente de ser atendida, se entregará al proceso. Y éste la deberá atender antes de que la función termine.

### 7.3.9 Función sigpending

La función `sigpending` regresa el conjunto de señales que están siendo bloqueadas y que, por lo tanto, no han sido entregadas al proceso. Este conjunto de señales es devuelto a través del argumento set.
```
#include <signal.h>
int sigpending(sigset_t * set);
```
 El siguiente programa empieza bloquea la señal `SIGQUIT`, guardando antes la máscara de señales (para poder restaurarla después) y luego se va a dormir por 5 segundos. Cualquier ocurrencia de la señal `quit` durante este periodo de tiempo es bloqueado y no se entregará al proceso hasta que la desbloquee.  Al final de los 5 segundos, reinstalamos la máscara original y, al mismo tiempo, checamos que existe una señal que esté pendiente de ser entregada.
```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigquit_handler(int sig) {
	fprintf(stdout, "Capturando SIGQUIT.\n");
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
		perror("No se puede restablecer SIGQUIT");
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	sigset_t newmask, oldmask, pendmask;
	
	if(signal(SIGQUIT, sigquit_handler) == SIG_ERR) {
		perror("no puede capturarse SIGQUIT");
		exit(1);
	}
	
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		perror("SIG_BLOCK");
		exit(1);
	}
	
	sleep(5);
	if (sigpending(&pendmask) < 0) {
		perror("sigpending");
		exit(1);
	}
	if (sigismember(&pendmask, SIGQUIT)) {
		fprintf(stdout, "\nSIGQUIT pendiente.\n");
	}
	
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		perror("SIG_SETMASK");
		exit(1);
	}
	fprintf(stdout, "\nSIGQUIT desbloqueado.\n");
	
	sleep(5);
	exit(0);
}
```
### 7.3.10 Función sigsuspend

Anteriormente visto que es posible cambiar la máscara de señales para bloquear o desbloquear alguna en particular. Podemos usar esta técnicas para proteger regiones críticas de código que no queremos que sean interrumpidas por una señal. El siguiente código intenta hacer esto, para ellos que la señal a ocurrir es `SIGINT`.
```
sigset_t newmask, oldmask;

sigemptyset(&newmask);
sigaddset(&newmask, SIGINT);	

if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
	perror("SIG_BLOCK");
	exit(1);
}

/* región crítica de código */
	
if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
	perror("SIG_SETMASK");
	exit(1);
}

/* ventana abierta */
pause();
```
/* continua el proceso */
Si la señal es enviada al proceso mientras está bloqueada, la señal será entregada hasta que sea desbloqueada. Para la aplicación, esto puede parecer como si la señal ocurriera entre el desbloqueo y pause (dependiendo de cómo el kernel implementa las señales). Si esto sucede, o si la señal sucede realmente en ese punto, estamos en un problema. Cualquier ocurrencia de la señal en esta ventana de tiempo se pierde en el sentido de que quizás no se capture esa señal otra vez, haciendo que pause bloquee el proceso de manera indefinida. Este es otro problema que se tenía con las primeras implementaciones de señales.
Para corregir este problema, necesitamos de una función que nos permita reinicializar la máscara de señales y, al mismo tiempo, poner el proceso a dormir en una operación atómica. Esta característica nos la provee la función `sigsuspend`.
```
#include <signal.h>
int sigsuspend(const sigset_t *sigmask);
```
La máscara de señal del proceso es establecida con el valor al que hace referencia el apuntado `sigmask`. Entonces el proceso es suspendido hasta que una señal es capturada o hasta que ocurra una señal que termine el proceso. Si la señal es capturada, el control es pasado al manejador. Cuando el manejador termina, entonces `sigsuspend` termina y la máscara de señal del proceso es restablecida al valor que tenía hasta antes del `sigsuspend`.

Esta función no regresa un valor exitoso de terminación. Si la función regresa el control al proceso, siempre devuelve 1 y la variable `errno` es igualada a `EINTR` (indicando una llamada a sistema interrumpida).
 El siguiente programa es un ejemplo de su uso:
 ```
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigint_handler(int sig) {
	fprintf(stdout, "\nsig_int: señal recibida %d\n", sig);
}

int main(int argc, char *argv[]) {
	sigset_t newmask, oldmask, waitmask;
	
	fprintf(stdout, "inicia programa.\n");
	
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		perror("SIGINT");
		exit(1);
	}
	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);
	
	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
		perror("SIG_BLOCK");
		exit(1);
	}
	
	fprintf(stdout, "región crítica.\n");
	if (sigsuspend(&waitmask) != -1) {
		perror("WAIT");
		exit(1);
	}
	sleep(30);
	fprintf(stdout, "terminando región crítica.\n");
	
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
		perror("SIG_SETMASK");
		exit(1);
	}
	
	fprintf(stdout, "termina programa.\n");
	exit(0);
}
```
Al ejecutar este programa, tendremos siguiente comportamiento:
```
$ ./sigsuspend
inicia programa - PID 5249.
región crítica.
^C
sig_int: señal recibida 2
^C
^C
^C
terminando región crítica.

sig_int: señal recibida 2
termina programa.
```
Como pueden ver, mientras el proceso está en región crítica la señal es bloqueada. Es hasta después de que termina la región, cuando se vuelve a aceptar la señal de SIGINT.















