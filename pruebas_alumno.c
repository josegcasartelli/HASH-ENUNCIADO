#include "pa2m.h"
#include "src/hash.h"
#include <stdlib.h>
#include <string.h>

static char *clave1 = "pikachu";
static char *clave2 = "bulbasuar";
static char *clave3 = "charmander";

void pruebas_null()
{
	pa2m_nuevo_grupo("Pruebas con NULL");

	pa2m_afirmar(hash_crear(0) == NULL,
		     "Crear hash con sin capacidad devuelve NULL");
	pa2m_afirmar(hash_cantidad(NULL) == 0, "Cantidad de un hash null es 0");
	pa2m_afirmar(!hash_insertar(NULL, "clave", NULL, NULL),
		     "Insertar en hash NULL falla");
	pa2m_afirmar(hash_buscar(NULL, "clave") == NULL,
		     "Buscar en hash null devuelve NULL");
	pa2m_afirmar(!hash_contiene(NULL, "clave"),
		     "Contiene en hash null devuelve false");
	pa2m_afirmar(hash_quitar(NULL, "clave") == NULL,
		     "Quitar en hash NULL devuelve NULL");
	pa2m_afirmar(hash_iterar(NULL, NULL, NULL) == 0,
		     "Iterar en hash NULL devuelve 0");
	hash_destruir(NULL);
	hash_destruir_todo(NULL, NULL);
}

void pruebas_crear()
{
	pa2m_nuevo_grupo("Pruebas crear");

	hash_t *hash = hash_crear(10);
	pa2m_afirmar(hash != NULL, "Se puede crear hash con capacidad valida");
	pa2m_afirmar(hash_cantidad(hash) == 0,
		     "Hash recien creado tiene cantidad 0");
	hash_destruir(hash);
}

void pruebas_insertar_y_buscar()
{
	pa2m_nuevo_grupo("Pruebas insertar y buscar");

	hash_t *hash = hash_crear(10);
	int valor = 40;
	pa2m_afirmar(hash_insertar(hash, clave1, &valor, NULL),
		     "Se puede insertar en hash");
	pa2m_afirmar(hash_cantidad(hash) == 1,
		     "Cantidad es uno despues de insertar 1");

	int *buscado = hash_buscar(hash, clave1);
	pa2m_afirmar(buscado != NULL && *buscado == 40,
		     "Buscar devuelve el valor correcto");

	int nuevo_valor = 50;
	void *anterior = NULL;
	hash_insertar(hash, clave1, &nuevo_valor, &anterior);
	pa2m_afirmar(&valor == anterior,
		     "Insertar clave existente reemplaza anterior");
	pa2m_afirmar(*(int *)hash_buscar(hash, clave1) == 50,
		     "Valor actualizado correctamente");

	hash_destruir(hash);
}

void pruebas_contiene_y_quitar()
{
	pa2m_nuevo_grupo("Pruebas contiene y quitar");

	hash_t *hash = hash_crear(10);
	int v1 = 1, v2 = 2;

	hash_insertar(hash, clave1, &v1, NULL);
	hash_insertar(hash, clave2, &v2, NULL);

	pa2m_afirmar(hash_contiene(hash, clave1), "hash conetiene clave 1");
	pa2m_afirmar(hash_contiene(hash, clave2), "hash conetiene clave 2");

	void *eliminado = hash_quitar(hash, clave1);
	pa2m_afirmar(eliminado == &v1, "Quitar devuelve el valor correcto");
	pa2m_afirmar(!hash_contiene(hash, clave1),
		     "clave1 ya no esta en el hash");
	pa2m_afirmar(hash_cantidad(hash) == 1, "cantidad actualizada");

	pa2m_afirmar(hash_quitar(hash, "clave_no_existente") == NULL,
		     "Quitar clave que no existe devuelve NULL");

	hash_destruir(hash);
}

bool iterar_suma(char *clave, void *valor, void *extra)
{
	(*(int *)extra) += *(int *)valor;
	return true;
}

bool cortar_en_dos(char *clave, void *valor, void *extra)
{
	int *contador = extra;
	(*contador)++;
	return *contador < 2;
}

void pruebas_iterador_interno()
{
	pa2m_nuevo_grupo("Pruebas iterador interno");

	hash_t *hash = hash_crear(10);
	int v1 = 10, v2 = 20, v3 = 30;

	hash_insertar(hash, clave1, &v1, NULL);
	hash_insertar(hash, clave2, &v2, NULL);
	hash_insertar(hash, clave3, &v3, NULL);

	int suma = 0;
	pa2m_afirmar(hash_iterar(hash, iterar_suma, &suma) == 3,
		     "Itera por todos los elementos");
	pa2m_afirmar(suma == 60, "Suma correctamente con iterador");

	int contador = 0;
	pa2m_afirmar(hash_iterar(hash, cortar_en_dos, &contador) == 2,
		     "Iterador corta correctamente");

	hash_destruir(hash);
}

void pruebas_rehash()
{
	pa2m_nuevo_grupo("Pruebas rehash automatico");

	hash_t *hash = hash_crear(2);
	int valores[10];
	char *claves[] = { "clave0", "clave1", "clave2", "clave3", "clave4",
			   "clave5", "clave6", "clave7", "clave8", "clave9" };

	for (int i = 0; i < 10; i++) {
		valores[i] = i * 10;
		pa2m_afirmar(hash_insertar(hash, claves[i], &valores[i], NULL),
			     "Se insertar con rehash");
	}

	pa2m_afirmar(hash_cantidad(hash) == 10,
		     "cantidad final despues de rehash es correcta");
	hash_destruir(hash);
}

void destructor_valores_aux(void *valor)
{
	free(valor);
}

void pruebas_destruir_y_destruir_todo()
{
	hash_t *hash1 = hash_crear(5);
	int v1 = 1, v2 = 2;
	hash_insertar(hash1, clave1, &v1, NULL);
	hash_insertar(hash1, clave2, &v2, NULL);
	hash_destruir(hash1);
	pa2m_afirmar(true, "hash destruido correctamente");

	hash_t *hash2 = hash_crear(5);
	int *a1 = malloc(sizeof(int));
	int *a2 = malloc(sizeof(int));
	*a1 = 10;
	*a2 = 20;
	hash_insertar(hash2, clave1, a1, NULL);
	hash_insertar(hash2, clave2, a2, NULL);
	hash_destruir_todo(hash2, destructor_valores_aux);
	pa2m_afirmar(true, "hash destruido con destructor correctamente");
}

int main()
{
	pruebas_null();
	pruebas_crear();
	pruebas_insertar_y_buscar();
	pruebas_contiene_y_quitar();
	pruebas_iterador_interno();
	pruebas_rehash();
	pruebas_destruir_y_destruir_todo();

	return pa2m_mostrar_reporte();
}
