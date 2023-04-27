
#ifndef PAGINACION_H_
#define PAGINACION_H_

#include "../../../shared/logger.h"
#include "../inicializacion/inicializacion.h"
#include "./clock.h"
#include "./clock_m.h"

int crear_estructura_proceso_nuevo(int pid, int tamanio);

/**
 * @NAME: get_pagina_n2
 * @DESC: Devuelve el indice de la pagina de nivel 2 a la que apunta la pagina de nivel 1
 * @PARAMS: indice de la direccion de nivel 1 a consultar
 * @RETURNS: indice de la direccion de nivel 2 en la direccion de nivel consultada
 */
int get_pagina_n2(int nro_tabla, int direccion_n1);

/**
 * @NAME: get_marco_de_pagina
 * @DESC: Devuelve el marco de la pagina de nivel 2
 * @PARAMS: indice de la direccion de nivel 2 a consultar
 * @RETURNS: nro de marco de la pagina de nivel 2 consultada
 */
int get_marco_de_pagina(int nro_tabla, int direccion_n2);

/**
 * @NAME: get_marco
 * @DESC: Devuelve la info que hay en un marco
 * @PARAMS: indice del marco a consultar
 * @RETURNS:info en el marco consultado
 */
void* get_marco(int nro_marco);

int meter_paginas_en_tablas(void* pagina, int direccion);

int guardar_proceso(void* proceso);

int cerrar_proceso(int indice);

int sacar_de_memoria(int indice, bool cierre);

#endif /* PAGINACION_H_ */
