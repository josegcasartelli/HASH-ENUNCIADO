#include "aux.h"
#include "hash_interno.h"
#include <stdlib.h>
#include <string.h>

//Auxiliares del hash

int comparar_claves(const void *a, const void *b)
{
	const par_t *par1 = a;
	const par_t *par2 = b;
	return strcmp(par1->clave, par2->clave);
}

size_t funcion_hash(char *clave, size_t capacidad)
{
	size_t hash = 5381;
	while (*clave)
		hash = hash * 33 + (unsigned char)(*clave++);
	return hash % capacidad;
}

bool rehash(hash_t *hash)
{
	if (!hash)
		return false;

	size_t nueva_capacidad = hash->capacidad * 2;
	lista_t **nueva_tabla = calloc(nueva_capacidad, sizeof(lista_t *));
	if (!nueva_tabla)
		return false;

	for (size_t i = 0; i < nueva_capacidad; i++) {
		nueva_tabla[i] = lista_crear();
		if (!nueva_tabla[i]) {
			for (size_t j = 0; j < i; j++)
				lista_destruir(nueva_tabla[j]);
			free(nueva_tabla);
			return false;
		}
	}

	for (size_t i = 0; i < hash->capacidad; i++) {
		lista_t *viejo_bucket = hash->tabla[i];
		while (lista_cantidad(viejo_bucket) > 0) {
			par_t *par = (par_t *)lista_eliminar_elemento(
				viejo_bucket, 0);
			size_t nuevo_indice =
				funcion_hash(par->clave, nueva_capacidad);
			if (!lista_agregar(nueva_tabla[nuevo_indice], par)) {
				for (size_t k = 0; k < nueva_capacidad; k++)
					lista_destruir(nueva_tabla[k]);
				free(nueva_tabla);
				return false;
			}
		}
		lista_destruir(viejo_bucket);
	}
	free(hash->tabla);
	hash->tabla = nueva_tabla;
	hash->capacidad = nueva_capacidad;

	return true;
}

//Auxiliares del main

bool insertar_en_hash(struct pokemon *pokemon, void *hash_void)
{
	hash_t *hash = hash_void;
	return hash_insertar(hash, pokemon->nombre, pokemon, NULL);
}

bool buscar_pokemon_por_id(char *clave, void *valor, void *extra)
{
	ctx_busqueda_id_t *contexto = extra;
	struct pokemon *p = valor;

	if (p->id == contexto->id) {
		contexto->encontrado = p;
		return false;
	}
	return true;
}
