#include <stdbool.h>
#include <stddef.h>
#include "hash.h"
#include "tp1.h"

int comparar_claves(const void *a, const void *b);

size_t funcion_hash(char *clave, size_t capacidad);

bool rehash(hash_t *hash);

typedef struct {
	int id;
	struct pokemon *encontrado;
} ctx_busqueda_id_t;

bool insertar_en_hash(struct pokemon *pokemon, void *hash_void);

bool buscar_pokemon_por_id(char *clave, void *valor, void *extra);