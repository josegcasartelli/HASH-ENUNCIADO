#include "src/hash.h"
#include "src/tp1.h"
#include "src/aux.h"
#include "src/hash_interno.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 5) {
		fprintf(stderr, "uso incorrecto");
		return 1;
	}

	char *archivo = argv[1];
	char *comando = argv[2];
	char *tipo = argv[3];
	char *valor = argv[4];

	if (strcmp(comando, "buscar") != 0) {
		fprintf(stderr, "comando incorrecto");
		return 1;
	}

	tp1_t *tp = tp1_leer_archivo(archivo);
	if (!tp) {
		fprintf(stderr, "error al leer el archivo");
		return 1;
	}

	hash_t *hash = NULL;

	if (strcmp(tipo, "nombre") == 0) {
		hash = hash_crear(3);
		if (!hash) {
			fprintf(stderr, "error al crear hahs");
			tp1_destruir(tp);
			return 1;
		}
		tp1_con_cada_pokemon(tp, insertar_en_hash, hash);

		struct pokemon *p = hash_buscar(hash, valor);

		if (p) {
			printf("Pokemon encontrado por nombre: ID=%d, Nombre=%s, Tipo=%d, Ataque=%d, Defensa=%d, Velocidad=%d\n",
			       p->id, p->nombre, p->tipo, p->ataque, p->defensa,
			       p->velocidad);
		} else {
			printf("No se encontro el pokemon con nombre %s\n",
			       valor);
		}

	} else if (strcmp(tipo, "id") == 0) {
		hash = hash_crear(3);
		if (!hash) {
			fprintf(stderr, "error al crear hash");
			tp1_destruir(tp);
			return 1;
		}
		tp1_con_cada_pokemon(tp, insertar_en_hash, hash);

		int id_buscado = atoi(valor);
		ctx_busqueda_id_t contexto = { .id = id_buscado,
					       .encontrado = NULL };
		hash_iterar(hash, buscar_pokemon_por_id, &contexto);
		struct pokemon *p = contexto.encontrado;
		if (p) {
			printf("Pokemon encontrado por id: ID=%d, Nombre=%s, Tipo=%d, Ataque=%d, Defensa=%d, Velocidad=%d\n",
			       p->id, p->nombre, p->tipo, p->ataque, p->defensa,
			       p->velocidad);
		} else {
			printf("No se encontro el pokemon con id %d\n",
			       id_buscado);
		}
	} else {
		fprintf(stderr,
			"tipo de busqueda incorrecta, use nombre o id\n");
	}

	hash_destruir(hash);
	tp1_destruir(tp);

	return 0;
}
