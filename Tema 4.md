
# 4. MANEJO DE DIRECTORIOS Y ARCHIVOS ESPECIALES

## 4.1 ACCESO A DIRECTORIOS
Los directorios son los archivos que le proporcionan al sistema la estructura jerárquica de árbol invertido.
Estos archivos tienen una estructura de datos que es interpretada y mantenida por el kernel. Los directorios,
como cualquier otro archivo, se pueden abrir mediante una llamada a `open` y pueden ser leídos mediante la
llamada `read`, pero a ningún usuario le está permitido escribir directamente sobre ellos mediante llamadas
a `write`.
Para leer la información de un directorio, debemos conocer su estructura. Como la organización de los datos
de un directorio depende del sistema, los programas que escribamos para manejarlos no van a ser
transportables. Para solucionar este inconveniente y hacer que la estructura del directorio sea transparente
al programador de aplicaciones, se ha propuesto una biblioteca estándar de funciones de manejo de
directorios. Esta biblioteca la estudiaremos más adelante.
Aunque no está permitido modificar directamente la estructura de un directorio, hay una serie de llamadas
que van a permitir crear o borrar directorios y añadirles nuevas entradas, lo cual es una vía indirecta de
modificarlos.
### 4.1.1 CREACIÓN DE UN DIRECTORIO – MKNOD Y MKDIR
Hemos visto que para crear un archivo ordinario disponemos de dos llamadas, una de ellas es `open`, con el
indicador `O_CREAT` activo, y la otra es `creat`, pero en Unix hay otros tres tipos de archivos: directorios,
archivos especiales y pipes. Para crear archivos de estos tres tipos se utiliza la función mknod. Su declaración
es como sigue:
```c
#include <sys/types.h>
#include <sys/stat.h>
int mknod(const char *path, mode_t mode, int dev);
```
`mknod` crea un archivo cuya ruta es la indicada por el parámetro `path`. El modo del archivo viene
especificado en el parámetro `mode`. El significado de este parámetro es el mismo que vimos para la llamada
`chmod`. En el archivo cabecera `<sys/stat.h>` hay definidas unas constantes simbólicas que podemos
utilizar para construir el parámetro mode mediante el operador `OR` (`|`) a nivel de bits. A continuación se
muestran estas constantes:

|Constante |Significado                        |
|--------- |:---------:                        |
|`S_IFMT`  | Tipo de archivo                   |
|`S_IFMT`  | Directorio                        |
|`S_IFREG` | Ordinario                         |
|`S_IFDIR` | Directorio                        |
|`S_IFCHR` | Especial modo carácter            |
|`S_IFBLK` | Especial modo bloque              |
|`S_IFIFO` | FIFO                              |
|`S_IFSOCK`| Conecto (Socket)                  |
|`S_ISUID` | Activar ID del usuario al ejecutar|
|`S_ISGID` | Activar ID del grupo al ejecutar  |
|`S_ISVTX` | Stiky Bit                         |
|`S_IRUSR` | Permiso de lectura para el usuario|
|`S_IWUSR` | Permiso de escritura para el usuario|
|`S_IXUSR` | Permiso de ejecución para el usuario|
|`S_IRGRP` | Permiso de lectura para el grupo|
|`S_IWGRP` | Permiso de escritura para el grupo|
|`S_IXGRP` | Permiso de ejecución para el grupo|
|`S_IROTH` | Permiso de lectura para otros|
|`S_IWOTH` | Permiso de escritura para otros|
|`S_IXOTH` | Permiso de ejecución para otros|

El parámetro `dev` tiene aplicación únicamente cuando vamos a crear un archivo de dispositivo (modo
bloque o carácter), siendo ignorado en el resto de los casos. `dev` codifica el “`major number`” y el
“`minor number`” del dispositivo y es dependiente de la implementación y de la configuración del
sistema. `dev` puede ser creado con la macro `makedev`, definida en `<sys/sysmacros.h>`.

`mknod` sólo puede ser invocado por un usuario con privilegios de superusuario, excepto a la hora de crear
un pipe, situación en la que cualquier usuario puede usar `mknod`. Por este motivo, `mknod` no es muy útil
para crear nuevos directorios.

Si `mknod` se ejecuta satisfactoriamente, devuelve el valor 0; en caso contrario, devuelve -1 y en `errno`
estará el código de error producido.

Para superar los problemas que plantea `mknod` a la hora de crear directorios, algunos sistemas suministran
la llamada `mkdir`. Su declaración es la siguiente:
```c
#include <sys/stat.h>
int mkdir(const char *path, mode_t mode);
```
`mkdir` crea un nuevo archivo directorio cuya ruta es la indicada en el parámetro path. Los bit de permiso
del nuevo directorio son inicializados de acuerdo con el parámetro mode y son modificados por la máscara
de modo de creación del proceso (ver `umask(2)`). Cada bit activado en la máscara de modo de creación
aparecerá desactivado en la máscara de modo del directorio recién creado.

Las constantes simbólicas definidas en `<sys/stat.h>` y referidas a los bits de permiso (desde `S_IRUSR`,
permiso de lectura de propietario, hasta `S_IXOTH`, permiso de ejecución por otros) pueden utilizarse para
crear el parámetro modo que le pasamos a `mkdir`.

Si `mkdir` se ejecuta satisfactoriamente, devuelve el valor 0; en caso contrario, devolverá -1 y en `errno`
estará el código del error producido.

`mkdir` puede escribirse a partir de `mknod` y cualquier usuario con los privilegios de superusuario podrá
ejecutarlo de esta forma. Una posible secuencia de código para `mkdir` es:

```c
#include <sys/stat.h>
#include <sys/types.h>
int mi_mkdir(const char *path, mode_t mode) {
  return mknod(path, S_IFDIR | mode, 0);
}
```
Desde la línea de comandos del sistema podemos crear directorios invocando el programa estándar `mkdir`.
