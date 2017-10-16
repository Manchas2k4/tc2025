# IMPLEMENTACIÓN DE UN POOL DE THREADS EN JAVA
## OBJETIVOS
Durante esta actividad el alumno será capaz de:

* Implementar un pool para administrar un número predefinido de `threads` utilizando la plataforma Java.

Esta actividad promueve las siguientes habilidades, valores y actitudes: análisis y síntesis, capacidad de resolver problemas, creatividad, y uso eficiente de la informática y las telecomunicaciones.

## DESCRIPCIÓN DE LA ACTIVIDAD
**Esta actividad puede ser elaborada de manera individual o en parejas.**

Toma el código fuente del [servidor de web](https://github.com/Manchas2k4/advanced_programming/blob/master/documents/temp/Pool_con_Java/webserver.zip) y añádele tu propia implementación de un _pool_ de `MAX_POOL_SIZE` _THREADS_. El _pool_ es una estructura de datos FIFO (_first-in first-out_). El propósito de usar un pool de _threads_ es evitar la creación y destrucción de un _thread_ cada vez que llega una nueva petición de un cliente. Los _threads_ deben ser creados una sola vez y, en lugar de dejarlos morir, reciclarlos de manera indefinida.

Esta es la descripción general de cómo debe funcionar el _pool_ de _threads_: Al llegar una nueva petición, se toma el primer _thread_ disponible en el _pool_ y se utiliza para procesar la petición. Cuando el _thread_ termine su procesamiento, se debe poner en estado de “wait” y colocarse al final del _pool_. Si en un momento dado llega una petición de un cliente y no existe un _thread_ disponible en el _pool_, el _thread_ principal debe responder con un código `503 Service Unavailable`.

## ¿QUÉ SE DEBE ENTREGAR?
En la parte superior de los archivos fuente de Java coloca en comentarios tus datos personales. Por ejemplo:

```c
/*----------------------------------------------------------------
 *
 * Actividad de programación: Implementación de un pool de threads
 * Fecha: 9-Sep-2015
 * Autor: 123456 Johnny Bravo
 *
 *--------------------------------------------------------------*/
 ```

Coloca en un archivo tarball llamado `tarea2.tgz` todos los archivos fuentes de tu programa así como el reporte del _speedup_ en formato PDF.

Sube el archivo tarball a Blackboard, en la sección de “Envío de tareas”.

**Fecha límite:** Miércoles, 16 de septiembre.

## EVALUACIÓN
Esta actividad será evaluada utilizando los siguientes criterios:

100 |	La actividad cumple con todos los requerimientos.
--- | :---
-10 |	No se incluyó en comentario los datos del autor.
10	| El programa fuente produce uno o más errores al momento de compilarlo.
50-90	| El programa funciona, pero produce algunos errores a tiempo de ejecución y/o los resultados no son del todo correctos.
DA |	La solución es un plagio.
