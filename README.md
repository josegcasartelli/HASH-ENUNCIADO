<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Repositorio de (Jose Gonzalez Casartelli) - (113633) - (jgcasartelli06@gmail.com)

- Para compilar:

```bash
make pruebas_alumno
```

- Para ejecutar:

```bash
./pruebas_alumno
```

- Para ejecutar con valgrind:
```bash
make valgrind-alumno
```
---
##  Funcionamiento

Explicación de cómo funcionan las estructuras desarrolladas en el TP y el funcionamiento general del mismo.

Aclarar en esta parte todas las decisiones que se tomaron al realizar el TP, cosas que no se aclaren en el enunciado, fragmentos de código que necesiten explicación extra, etc.

Incluir **EN TODOS LOS TPS** los diagramas relevantes al problema (mayormente diagramas de memoria para explicar las estructuras, pero se pueden utilizar otros diagramas si es necesario).



---

## Respuestas a las preguntas teóricas
Incluír acá las respuestas a las preguntas del enunciado (si aplica).

Explicá teóricamente los siguientes puntos (no necesariamente en orden, pero por favor usando diagramas):

   - Qué es un diccionario
    - Explicá 3 formas diferentes de implementar un diccionario (tabla de hash cuenta como 1 sola implementación posible)
   - Qué es una función de hash y qué características debe tener para nuestro problema en particular
   - Qué es una tabla de Hash y los diferentes métodos de resolución de colisiones vistos (encadenamiento, probing, etc)
   - Explique por qué es importante el tamaño de la tabla (tanto para tablas abiertas como cerradas)
     - Dado que en una tabla abierta se pueden encadenar colisiones sin importar el tamaño de la tabla, ¿Realmente importa el tamaño?
   - Mas te vale que expliques con dibujos



Un diccionario es un TDA que mantiene pares clave-valor con claves únicas.  Permite operaciones para insertar, buscar, verificar si una clave existe y quitarla.  La eficiencia depende de la estructura interna elegida.  

Hay varias maneras de implementar un diccionario.  Una muy común es la tabla de hash donde se aplica una función de hash sobre la clave para obtener un índice y se guarda el par en el bucket correspondiente.  Otra alternativa es un arbol binario de busqueda balanceado (AVL o rojo-negro).  En este caso las claves se mantienen ordenadas y las operaciones insertar, buscar y eliminar cuestan O(log n) en el peor caso.  Una tercera opción ampliamente usada en bases de datos es el árbol B.  Este árbol tiene poca altura y permite almacenar muchas claves por bloque.  También tiene complejidad O(log n).

La función de hash es el procedimiento que transforma una clave (en este TP, un string no nulo)  en un índice dentro del arreglo de buckets.   Para este problema, debe ser rápida respecto de la longitud de la clave, distribuir de manera uniforme para evitar concentraciones, y comportarse bien frente a claves similares.

Una tabla de hash es, entonces, un arreglo de tamaño fijo (dada por la capacidad) de buckets.  La colisión aparece cuando dos claves distintas caen en el mismo bucket.  Hay dos formas de resolver colisiones.  El primero es encadenamiento, donde cada bucket guarda una lista enlazada con todos los pares que mapean a ese índice.  El segundo es con probing y a diferencia de encadenamiento, todos los pares viven dentro del propio arreglo y, si el lugar primario está ocupado, se buscan posiciones alternativas libres.

El tamaño de la tabla importa en ambos enfoques porque determina el factor de carga (cantidad de elementos sobre capacidad).  En encadenamiento en una tabla abierta, si el factor de carga crece entonces las listas por bucket crecen y las comparaciones aumentan.  En probing en una tabla cerrada, a medida que el factor de carga se acerca a uno el rendimiento cae de forma abrupta por clustering.  Por eso se define una función rehash que cuando el factor de carga supera como en el caso de este tp 0.75, se crea una tabla más grande y se reubican todos los pares con la nueva capacidad.

En una tabla abierta si importa el tamaño.  Aunque el método permite crecer sin límites, en la práctica un factor de carga alto degrada el tiempo promedio.  Mantener el factor de carga bajo usando una tabla adecuada y la función rehash cuando sea necesario preserva una complejidad más cercano a O(1).




