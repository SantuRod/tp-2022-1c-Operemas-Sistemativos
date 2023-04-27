#include "inicializacion.h"

void levantar_estructura_paginacion();
void levantar_list_ctrl_memoria();

void inicializar_memoria(){
	indice_clock = 0;
    tam_memoria = get_tam_memoria();
    memoria = malloc(tam_memoria);
    tam_pagina = get_tam_pagina();
	cant_entradas_x_tabla = get_paginas_por_tabla();
    cant_total_marcos = tam_memoria / tam_pagina;
	cant_max_pag = cant_entradas_x_tabla * cant_entradas_x_tabla;
	cant_marcos_x_proceso = get_marcos_por_proceso();
	levantar_list_ctrl_memoria();
	loggear_trace("Estructura de memoria principal levantada con tamaño [%zu] bits y [%zu] marcos de [%zu] bits tamaño!", tam_memoria, cant_total_marcos, tam_pagina);
    levantar_estructura_paginacion();  
}

void levantar_estructura_paginacion() {

	list_tablas_paginas_nivel1 = list_create();
	list_tablas_paginas_nivel2 = list_create();
	indice_x_pid = list_create();
	loggear_trace("Estructura de paginacion levantada!");

}


void levantar_list_ctrl_memoria(){
	ctrl_memoria = list_create();

	for (int i = 0; i < cant_total_marcos; i++){
		t_info_marco* marco = malloc(sizeof(t_info_marco));
		marco->nro_marco = i;
		marco->ocupado = false;
		marco->indice = -1;
		list_add(ctrl_memoria, marco);
	}
}

void destruir_memoria(){
	list_destroy_and_destroy_elements(ctrl_memoria, (void*) free);
	free(memoria);
	
	void tablas_paginas_destroy(t_list *self) {
		list_destroy_and_destroy_elements(self, (void*) free);
	}

	list_destroy_and_destroy_elements(list_tablas_paginas_nivel2, (void*) tablas_paginas_destroy);
	list_destroy_and_destroy_elements(list_tablas_paginas_nivel1, (void*) tablas_paginas_destroy);
	
	list_destroy_and_destroy_elements(indice_x_pid, (void*) free);
}


