
#ifndef INICIALIZACION_H_
#define INICIALIZACION_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <commons/config.h>
#include <commons/string.h>

#include "../../../shared/codigo_error.h"

#include "../configuracion/memoria_configuracion.h"

#include "../../../shared/estructura_compartida.h"


void* memoria;

size_t tam_memoria;
size_t tam_pagina;
uint32_t cant_total_marcos;
uint32_t cant_marcos_x_proceso;
uint32_t cant_entradas_x_tabla;
uint32_t cant_max_pag;

uint32_t indice_clock;

t_list* list_tablas_paginas_nivel1;
t_list* list_tablas_paginas_nivel2;
t_list* ctrl_memoria;
t_list* indice_x_pid;

typedef struct {
	uint32_t nro_marco;
	bool ocupado;
	uint32_t indice;
} t_info_marco;


typedef struct {
	uint32_t nro_pagina;
	uint32_t marco;
	bool bit_presencia;
	bool bit_uso;
	bool bit_modificado;
} t_pagina_nivel2;

/**
 * @NAME: inicializar_memoria
 * @DESC: Inicializa la memoria y todos las estrucuturas que va a utilizar
 */
void inicializar_memoria();

void destruir_memoria();

#endif /* INICIALIZACION_H_ */
