![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_a.png "Actividades")

# Actividad: Laboratorio - Actividad sobre creación de procesos.

# Módulo: Administración de procesos.


## Descripción

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_b.png "Descripción") 
* Durante la sesión, de manera individual, deberás desarrollar un programa que nos permite crear procesos de una forma muy particular.

## Modalidad

![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_c.png "Modalidad") 
* Individual.

## Objetivos de la actividad
* Utilizar las llamadas a sistema relacionadas con la administración de procesos.

## Instrucciones
  
![alt text](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/images/d_d.png "Instrucciones")
  
  Implementa la solución del siguiente problema:
  
  Escribe un programa llamado `descendiendo`. El programa recibe de línea de comando un número entero. La forma de invocar el programa será:

    `$ descendiendo num`

   El programa generará el siguiente árbol de procesos: el proceso inicial creará 1 hijo, le llamaremos NIVEL1; NIVEL1 creará dos NIVEL2; cada NIVEL2 creará 3 hijos NIVEL3; y así sucesivamente NIVELN-1 creará N hijos. Cada proceso deberá desplegar información sobre el id de su padre y su propio id. Cada proceso deberá esperar hasta que todos sus hijos hayan terminado.

   Ejemplo:

```
$ descendiendo
forma de uso: descendiendo num

$descendiendo a
error: el programa recibe un numero entero positivo mayor a 0.

$descendiendo -1
error: el programa recibe un numero entero positivo mayor a 0.

$descendiendo 0
error: el programa recibe un numero entero positivo mayor a 0.

$ descendiendo 3
PPID = 1234 PID = 1235 NIVEL = 0
  PPID = 1235 PID = 1236 NIVEL = 1
    PPID = 1236 PID = 1237 NIVEL = 2
      PPID = 1237 PID = 1238 NIVEL = 3
      PPID = 1237 PID = 1239 NIVEL = 3
      PPID = 1237 PID = 1240 NIVEL = 3
    PPID = 1236 PID = 1241 NIVEL = 2
      PPID = 1241 PID = 1242 NIVEL = 3
      PPID = 1241 PID = 1243 NIVEL = 3
      PPID = 1241 PID = 1244 NIVEL = 3
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
