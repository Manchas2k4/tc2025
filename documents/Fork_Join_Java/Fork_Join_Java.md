# PROCESANDO IMÁGENES CON EL FORK/JOIN FRAMEWORK DE JAVA
## OBJETIVOS
Durante esta actividad el alumno será capaz de:

*Utilizar el Fork/Join _framework_ de Java 7 para paralelizar eficientemente el procesamiento de una imagen digital.

Esta actividad promueve las siguientes habilidades, valores y actitudes: análisis y síntesis, capacidad de resolver problemas, creatividad, y uso eficiente de la informática y las telecomunicaciones.

## DESCRIPCIÓN DE LA ACTIVIDAD
**Esta actividad debe ser elaborada de manera individual.**

Escribe dos versiones de un programa en Java que convierta una imagen PNG de color a escala de grises. La primera versión debe hacerlo de forma secuencial; la segunda versión debe hacerlo en paralelo utilizando la clase `java.util.concurrent.RecursiveAction` del Fork/Join _framework_. Compara varios tiempos de ejecución de ambas versiones y realiza un breve reporte en donde documentes el [speedup]() obtenido.

Usa estos factores al momento de hacer la conversión a escala de grises: Rojo 29.9%, Verde 58.7% y Azul 11.4%.

Puedes probar tu programa con las siguientes imágenes:

* [natalie1.jpg]() (1590×1143 pixeles, 1.7 MB)
* [natalie2.png]() (2560×1600 pixeles, 4.1 MB) 

## ¿QUÉ SE DEBE ENTREGAR?
En la parte superior de los archivos fuente de Java coloca en comentarios tus datos personales. Por ejemplo:
```
/*----------------------------------------------------------------
 *
 * Actividad de programación: Fork-join framework
 * Fecha: 30-Sep-2015
 * Autor: 123456 Johnny Bravo
 *
 *--------------------------------------------------------------*/
 ```

Coloca en un archivo tarball llamado `tarea3.tgz` todos los archivos fuentes de tu programa así como el reporte del speedup en formato PDF.

Sube el archivo tarball a Blackboard, en la sección de “Envío de tareas”.

**Fecha límite:** Miércoles, 30 de septiembre.

## EVALUACIÓN
Esta actividad será evaluada utilizando los siguientes criterios:

100	| La actividad cumple con todos los requerimientos.
:--- | :---
-10	| No se incluyó en comentario los datos del autor.
10	| El programa fuente produce uno o más errores al momento de compilarlo.
50-90	| El programa funciona, pero produce algunos errores a tiempo de ejecución y/o los resultados no son del todo correctos.
DA	| La solución es un plagio.
