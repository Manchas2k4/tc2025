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
