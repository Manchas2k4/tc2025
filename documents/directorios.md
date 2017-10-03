![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_a.png "Actividades")

# Actividad: Laboratorio - Actividad sobre directorios.

# Módulo: Manejo de archivos y directorios.


## Descripción

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_b.png "Descripción") 
* Durante la sesión, de manera individual, deberás desarrollar un programa que, a partir de las entradas de un archivo tipo directorio, obtiene información estadística de su contenido.

## Modalidad

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_c.png "Modalidad") 
* Individual.

## Objetivos de la actividad
* Utilizar las llamadas a sistema relacionadas con la manipulación de archivos tipo directorio.

## Instrucciones
  
![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_d.png "Instrucciones")
  
  Implementa la solución del siguiente problema:
  
  Escribe un programa llamado `estadistica` que despliega recursivamente la información sobre el tipo de archivos que hay en un directorio. La forma de invocar el programa será:

    `$ estadistica [directorio]`

   El programa deberá mostrar los tipos de archivos que hay en el directorio (y sus subdirectorios). El porcentaje que presentan siempre es con respecto al total de archivos de ese directorio en particular. El directorio por omisión es el directorio de trabajo.

   Ejemplo:

```
    $ estadistica
    
    Directorio /home/mi_usuario 
    
    Tipo            Porcentaje 
    
    REG             80%
    
    DIR             10%
    
    FIFO            5%
    
    LNK             5%
```

```
    Directorio /home/mi_usuario/Música 
    
    Tipo            Porcentaje
    
    REG             99%
    
    DIR             1%
```

```
    $ estadistica mi_usuario/Música 
    
    Directorio /home/mi_usuario/Música 
    
    Tipo            Porcentaje
    
    REG             99%
    
    DIR             1%
```

``` 
    $ estadistica no_existe
    
    opendir: el directorio no existe.
```

## Recursos

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_e.png "Recursos")
* No aplica.

## Especificaciones de entrega

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_f.png "Especificaciones")
* Enviar a través de la sección de "Tareas" de Blackboard.


<br>
<br>

**DR© Tecnológico de Monterrey Campus Querétaro| Departamento de Desarrollo Académico| México, 2012**
