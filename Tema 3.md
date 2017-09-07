# 3. MANEJO DE ARCHIVOS
## 3.1 INTRODUCCIÓN
En el tema anterior hemos delineado la estructura interna del sistema de archivos de Unix. En este tema vamos a estudiar la interfaz que ofrece el sistema para comunicarnos con el kernel y poder acceder a los recursos del sistema de archivos. Más conretamente, la comunicación se va a realizar con una parte del kernel, el subsistema de archivos o susbsistema de entrada/salida. Trabajaremos con los archivos aprovechando la estructura de alto nivel que ofrece el sistema de archivos y permite realizar una abstracción de lo que es el soporte físico de la información (discos).

Para empezar, vamos a ver un ejemplo sencillo que ilustra cómo se comunica un programa con el subsistema de archivos. El programa siguiente es una versión simplificada de la orden `cp`, que permite copiar archivos. Lo llamaremos `copiar`.

```
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

char buffer[BUFSIZ];

int main(int argc, char *argv[]) {
       int fd_origen, fd_destino;
       int nbytes;
       
       if (argc != 3) {
              fprintf(stderr, "Forma de uso: %s archivo_origen archivo_destino\n", argv[0]);
              return -1;
        }
        
        if ((fd_origen = open(argv[1], O_RDONLY)) < 0 {
              fprintf(stderr, "Error con el archivo: %s", argv[1]);
              return -1;
        }
        
        if ((fd_destino = open(argv[2], O_WRONGLY|O_TRUNC|O_CREAT, 0666)) < 0 {
              fprintf(stderr, "Error con el archivo: %s", argv[2]);
              return -1;
        }
        while ((nbytes = read(fd_origen, buffer, sizeof(buffer))) > 0) {
               write(fd_destino, buffer, nbytes);
        }
        
        close(fd_origen);
        close(fd_destino);
        return EXIT_SUCCESS;
}
```
Este programa tan sencillo muestra el esquema general que se debe seguir para trabajar con archivos. La secuencia recomendada es:
* Abrir los archivos (llamada `open`) y comprobar si se produce algún error en la apertura. En caso de error, open devuelve el valor de -1.
* Manipular los archivos de acuerdo a nuestras necesidades. En este ejemplo leemos del archivo origen y escribimos en el archivo destino. Hay que hacer notar que la lectura/escritura se realiza en bloques de tamaño `BUFSIZ` (constante definida en `<stdio.h>`). La forma de detectar que hemos leído todo el archivo origen es analizando el valor devuelto por `read`. Si este valor es igual a cero, significa que ya no quedan más datos que leer del archivo origen.
* Cerrar los archivos una vez que hemos terminado de trabajar con ellos.

Para kernel, todos los archivos abiertos son accedidos mediante descriptores. Un descriptor es un número entero no negativo. Cuando nosotros abrimos un archivo existente o creamos uno nuevo, el kernel nos regresa un descriptor para el programa (o proceso). Cuando queremos leer o escribir un archivo, nosotros usamos el archivo con el descriptor (que nos ha devuelto `open` o `creat`) como un parámetro de las funciones de `read` o `write`.

Por convención, los shells de Unix asocia el descriptor `0` a la entrada estándar de un programa, el descriptor `1` con la salida de datos estándar y el descriptor `2` con la salida de errores estándar. Esta convención es utilizada por shells y muchas aplicaciones; esto no es una característica del kernel de Unix.  Sin embargo, muchas aplicaciones pueden tener problemas al ejecutarse si no se siguen estas asociaciones. Los números mágicos `0`, `1` y `2` pueden ser reemplazados con las constantes `STDIN_FILENO`, `STDOUT_FILENO`, `STDERR_FILENO`. Estas constantes están definidas en el archivo `<unistd.h>`. El rango para los descriptores va desde `0` a `OPEN_MAX`. Históricamente el máximo de archivos permitidos por un proceso era de 20, pero muchos sistemas han incrementado este número hasta 63.

También suele ser convenio bastante extendido que un programa, cuando termina satisfactoriamente, devuelva el valor 0 al sistema operativo. Este valor es almacenado en la variable de entorno `$?`, que puede ser analizada por otro proceso para ver el código de error devuelto por el último proceso que la modificó. Algunas utilidades, como `make`, emplean estos códigos de error para determinar si deben proseguir su ejecución o deben detenerse. Desde la línea de instrucciones podemos ver el contenido de `$?` escribiendo:

`$ echo $?`

Para los códigos devueltos en caso de error no existe un criterio concreto, salvo el de que sean distintos de 0.

## 3.2 Entrada y Salida sobre Archivos Ordinarios
En los siguientes párrafos vamos a estudiar las llamadas al sistema necesarias para realizar entrada/salida sobre archivos ordinarios.

### Función open
`open` es la función que utilizaremos para indicarle al kernel que habilite las instrucciones necesarias para trabajar con un archivo que especificaremos mediante una ruta. El kernel devolverá un descriptor de archivos con el que podremos referenciar al archivo en funciones posteriores. La declaración de open es:

```
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open (const char *pathname, int oflag, …, mode_t mode);
```

Mostramos el tercer argumento como …, es el formato que utiliza ISO C para especificar que el número y tipos de los argumentos restante puede variar. Para esta función, el tercer argumento es usado solo cuando un nuevo archivo está siendo creado, como veremos más tar`pathname´ es la ruta del archivo que queremos abrir. Puede ser una ruta absoluta o relativa y su longitud no debe exceder de ´PATH_MAX´ bytes.

´oflags´ es una máscara de bits (varias opciones pueden estar presentes usando el operador OR a nivel de bits) que le indica al kernel el modo en que queremos que se abra el archivo. Uno de los bits, O_RDONLY, O_WRONLY u O_RDWR, y sólo uno, debe estar presente al componer la máscara; de lo contrario, el modo de apertura quedaría indefinido. Los ´oflags´ más significativos que hay disponibles son:


