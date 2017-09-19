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
| **Señal**     | **Significado*|    
| ------------- |:-------------:| 
| col 3 is      | Desconexión. Es enviada cuando una terminal se desconecta de todo proceso del que es terminal de control. También se envía a todos los procesos de un grupo cuando el líder del grupo termina su ejecución. La acción por defecto de esta señal es terminar la ejecución del proceso que la recibe.| 
| col 2 is      | centered      |   
| zebra stripes | are neat      |   

