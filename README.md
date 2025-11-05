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



El programa funciona abriendo el archivo pasado por parámetro y leyéndolo línea por línea y guardando un registro de cada pokémon en un vector.  Luego se fija en el tipo de búsqueda que se quiera hacer, ya sea por nombre o por id.  Si el tipo de busqueda es por nombre, crea un hash con capacidad inicial 3. Luego recorre todos los pokemones con tp1_con_cada_pokemon() y los insertar usando la funcion auxiliar insertar_en_hash().  La funcion recibe cada pokemon y lo insertar en la tabla hash usando su nombre como clave.  Una vez que todos los pokemones hayan sido insertados en el hash se llama a hash_buscar pasandole el nombre del pokemon como valor.  Si lo encuentra imprime sus datos, y si no, muestra el mensaje de error.  Si el tipo de busqueda es por id, al igual que antes crea un hash con capacidad inicial 3 y va llenando el hash al igual que en el anterior.  En este caso no podemos usar hash_buscar() porque el hash guarda las claves por nombre (char*).  Entonces lo que se hace es iterar el hash hasta encontrar el pokemon.  Se convierte el valor a un entero y se prepara una estructura que va ser usada como contexto para pasarle a hash_iterar().  En la estructura, contexto.id es el id que se quiere buscar, y contexto.encontrado empieza en NULL y va a quedar apuntando al pokemon cuando lo encontremos.  Se llama a hash_iterar(hash, buscar_pokemon_por_id, &contexto) en donde va recorrer todos los buckets y, dentro de cada lista, cada par (clave, valor).  Por cada par llama a la funcion callback buscar_pokemon_por_id y si se encontro guarda el putero en contexto->encontrado y devolvemos false para cortar el recorrido del iterador.  Al volver de hash_iterar, miras contexto.encontrado y si no es NULL imprimis los datos.


//Funciones del hash.h

La primera funcion es hash_crear().  Esta funcion inicializa una tabla vacia, con una cantidad fija de buckets que es pasada por parametro.  Si la capacidad es 0, devuelve NULL porque no tiene sentido tener una tabla vacia.  Si es menor a 3, fuerza la capacidad a 3, y luego reserva memoria para la estructura hash.  Una vez reservado espacio en el heap para el hash se reserva memoria para la tabla (el vector de buckets), y se utiliza calloc para inicializar todos los punteros en NULL.  Luego se utiliza un for que recorre hasta la capacidad del hash y crea una lista en cada indice de la tabla.  Si en algun momento algun bucket falla, se destruye los anteriores y se retorna NULL.  En cambio si se pudo crear todas las lista se devuelve un puntero al hash creado.  Esta funcion tiene complejidad O(n) ya que itera sobre todas las posiciones n de la tabla para poder crear una lista en cada una.


Hash cantidad simplemente devuelve cuantos elementos hay totales en la tabla.  Recibe un hash por parametro y si es NULL devuelve 0, si no devuelve el valor almacenado en el campo cantidad del hash.  La complejidad de esta funcion es O(1) porque solamente accede a un campo del struct.


Hash insertar tiene como objetivo insertar un par clave-valor en el hash o actualizarlo si ya existe.  Lo primero que se hace es controlar el factor de carga.  Se tomo como limite el 75% del hash ocupado, y entonces si la tabla esta en el limite o superior, llama a la funcion auxliar rehash() para crear una tabla nueva con mas espacio.  Una vez que haya espacio suficiente para insertar se calcula donde insertar el nuevo par usando la funcion hash que devuelve un indice.  Usando el indice obtiene el bucket(lista) correspondiente y busca si la clave ya existe.  Se crea una estructura temporal par_t con la clave buscada y se llama a lista_buscar_posicion().  Si la funcion devuelve un numero mayor igual a 0 entonces la clave existe y devuelve la posicion de la clave existente.  Esa posicion es el indice dentro del bucket, no el indice dentro de toda la tabla.  Entonces ahora simplemente puedo obtener un puntero directo al elemento usando lista_buscar_elemento() que recorre nuevamente la lista y devuelve el void* guardado ahi que luego se convierte a par_t*.  Por ultimo guardo el valor reemplazado en encontrado, y actualizo el valor del par_existente sin tocar la clave solamente actualizando el puntero al valor.  En el caso de que la clave no exista entonces se entra al bloque siguiente.  Aca se reserva memoria para un nuevo par que tendra su propia clave y un putero al valor.  Primero se reserva memoria para la clave y luego se copia la cadena completa.  El campo valor simplemente guarda el puntero recibido.  Despues se intenta insertar el nuevo par al final del bucket correspondiente.  En el caso de que no se pueda, se libera toda la memoria y se retorna false.  Por ultimo, como se inserto un par nuevo se incrementa la cantidad del hash, y encontrado queda en NULL porque como la clave es nueva no se reemplazo ningun valor.


La siguiente funcion es hash_buscar().  Esta recibe un hash y una clave y devuelve el valor asociado a esa clave.  Parecido a lo que se hizo antes, se calcula el indice con la funcion hash y recorre el bucket correspondiente.  Usa lista_buscar_elemento() y compara cada clave con la clave pasada por parametro usando strcmp.  Si son iguales entonces devuelve el valor, y si no, devuelve NULL.


Hash contiene reutiliza la funcion hash_buscar() pero esta vez la funcion es booleana y retorna true si la clave existe, y sino, false.


hash_quitar() elimina una clave del hash y devuelve su valor.  Calcula el indice del hash con la funcion hash y recorre el bucket buscando la clave.  Si lo encuentra entonces llama a lista_eliminar_elemento() para quitar el par_t, despues libera la clave copiada, libera la estructura par_t, baja la cantidad del hash por uno y retornar el valor eliminado. Si no la encuentra, devuelve NULL.


hash_iterar() aplica una funcion a cada elemento del hash y devuelve la cantidad de veces que se aplico esa funcion. La idea es usar un for para recorrer todas las posiciones del hash y para cada bucket crear un iterador de lista para recorrer todos los elementos del bucket actual.  Despues mientras hayan elementos en la lista se va a usar el iterador para obtener el elemento actual y aplicarle a la funcion f.  En caso de que no pueda destruye el iterador y devuelve la cantidad de veces que se aplico f, si no sigue iterando hasta que no hayan mas elementos para iterar y devuelve cantidad. Como se tiene que iterar sobre todos los elementos del hash una vez donde n es la cantidad total de elementos, se tiene una complejidad de O(n).


hash_destruir() libera toda la memoria asociada al hash.  Recorre cada bucket y mientras tenga elementos se libera la clave y el par.  Luego se destruye la lista vacia, libera la tabla y la estructura principal del hash.  No libera los valores par->valor porque podrian ser usados afuera y ademas para eso existe hash_destruir_todo().  La complejidad es O(n) porque tiene que recorrer todos los n elementos almacenados exactamente una vez.


hash_destruir_todo() es igual que hash_destruir con adicion de que tambien destruye los valores almacenados.  Si el destructor no es NULL, lo usa sobre cada par->valor y despues libera par->clave y par.  Finalmente, destruye todas las lista, la tabla y el hash.  La complejidad de esta funcion es O(1)al igual que hash_destruir() porque tiene que recorrer todos los elementos del hash.





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




