// Instancias obtenidas de la página:
https://site.unibo.it/operations-research/en/research/bpplib-a-bin-packing-problem-library

Sección Benchmarks que contiene 1615 instancias utilizadas en este problema en caso unidimensional.

Detalles de las instancias: 
Falkenauer (BPP - CSP)

These are the instances used by E. Falkenauer in 
A hybrid grouping genetic algorithm for bin packing. 
Journal of Heuristics, 2(1):5–30, 1996. 
They were downloaded from the OR-library. 
They are divided into two classes of 80 instances each:
the first class has uniformly distributed item sizes (‘Falkenauer U’) 
with n between 120 and 1000, and c = 150.

Se escogió la sección U120 donde el tamaño de los objetos está entre [20,100]
porque si crece, se puede complicar la solución. El rango del óptimo 
está entre [46,52] paquetes por contenedor. Cuya capacidad es de 150. 
Descripción de esta instancia se puede encontrar en el artículo: 
"A hybrid evolutionary algorithm for the offline Bin Packing Problem"
donde muestran que se resolvió para 80 instancias (20 de U120, 20 de U200, 
20 de U500, 20 de U1000) pero no muestran resultados específicos. 

En el archivo "Solutions-UT" copié sólo la parte de Falkenauer de 
"Solutions" que se encuentra en la página ya mencionada. 
Si LB = UB (creo que significa que si el lower bound -límite inferior- es igual
al uper bond -límite superior- el problema ya está resuelto. 
* Best LB obtained from the exact algorithms
* Best UB obtained from the exact algorithms
Y en "Solutions-UT" viene el resultado exacto. 


La primera línea contiene la cantidad de datos que hay y la segunda la capacidad del 
contenedor. 
¿Es como si cada contenedor fuera de máximo 150? Y habría en el primer caso
máximo 52 contenedores. 




