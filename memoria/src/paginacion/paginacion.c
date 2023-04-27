#include "paginacion.h"


bool _esta_en_marcos(t_pagina_nivel2* pagina){
    return pagina->bit_presencia;
}


int guardar_pagina(uint32_t indice, int nro_pagina){


    t_list* tabla_paginas_nivel2 = list_get(list_tablas_paginas_nivel2, indice);
    t_list* paginas_en_marco = list_filter(tabla_paginas_nivel2, (void*) _esta_en_marcos);
    loggear_trace("El proceso tiene %zu marcos ocupados", list_size(paginas_en_marco));
        
    if(list_size(paginas_en_marco) < cant_marcos_x_proceso){

        bool _marco_del_proceso(t_info_marco* marco){
            return marco->indice == indice;
        }

        t_list* marcos_del_proceso = list_filter(ctrl_memoria, (void*) _marco_del_proceso);
        t_info_marco* marco_usado = list_get(marcos_del_proceso, list_size(paginas_en_marco));
        loggear_trace("El proceso tiene marcos vacios, usa el marco %zu", marco_usado->nro_marco);
        cargar_pagina_en_memoria(indice, nro_pagina, marco_usado->nro_marco); 

        list_destroy(marcos_del_proceso);
        list_destroy(paginas_en_marco);
        return marco_usado->nro_marco;
    } else {
        list_destroy(paginas_en_marco);
        loggear_debug("No hay marcos libres, se utiliza el algoritmo de reemplazo [%s]", get_algoritmo_reemplazo());
        if(string_equals_ignore_case(get_algoritmo_reemplazo(), "CLOCK")) return reemplazar_clock(indice, nro_pagina);
        else return reemplazar_clock_m(indice, nro_pagina);
    }
    return -1;
}


int get_pagina_n2(int nro_tabla, int direccion_n1) {
    t_list* tabla_paginas_nivel1 = list_get(list_tablas_paginas_nivel1, nro_tabla);
    if(direccion_n1 >= list_size(tabla_paginas_nivel1)) {
        loggear_error("La direccion leida esta fuera de la tabla de paginas de nivel 1 de ese proceso ");
        return -1;
    }
    loggear_debug("Tomando la direccion de memoria de la tabla de nivel 1 con indice [%zu]", direccion_n1);
    usleep(get_retardo_memoria()*1000);
    int* direccion_n2 = list_get(tabla_paginas_nivel1, direccion_n1);

    loggear_debug("La tabla de paginas de nivel 2 consultada empieza en la direccion [%d] ", *direccion_n2);
    return *direccion_n2;
}

int get_marco_de_pagina(int nro_tabla, int direccion_n2) {
    t_list* tabla_paginas_nivel2 = list_get(list_tablas_paginas_nivel2, nro_tabla);
    if(direccion_n2 >= list_size(tabla_paginas_nivel2)) {
        loggear_error("La direccion leida esta fuera de la tabla de paginas de nivel 2 de ese proceso ");
        return -1;
    }
    loggear_debug("Tomando el marco de la pagina con indice [%zu]", direccion_n2);
    usleep(get_retardo_memoria()*1000);
    t_pagina_nivel2* pagina = list_get(tabla_paginas_nivel2, direccion_n2); 
    if(!pagina->bit_presencia) {
        loggear_info("La pagina no se encuentra en memoria");
        loggear_warning("=========================================================");
        loggear_warning("|                      Page fault                       |");
        loggear_warning("=========================================================");
        pagina->marco = guardar_pagina(nro_tabla, direccion_n2);
        pagina->bit_presencia = 1;
        pagina->bit_uso = 1;
    } else {
        loggear_debug("Se encontro que la pagina esta en el marco [%zu]", pagina->marco);
        pagina->bit_uso = 1;
    } 
    return pagina->marco;
}

int crear_estructura_proceso_nuevo(int pid, int tamanio){
    //Crea Tabla de paginas
    int cant_paginas = tamanio/tam_pagina;
    int extra = tamanio%tam_pagina;
    if(extra > 0) cant_paginas++;
    uint32_t indice_pagina = 0;
    loggear_trace("El proceso [%zu] necesita [%zu] paginas", pid , cant_paginas);

    if(cant_paginas > cant_max_pag){
        loggear_warning("El proceso [%zu] es mas grande que el maximo aceptado por memoria, se lo rechaza", pid);
        return -1; //El proceso es mas grande de lo que el sistema acepta
    } 

    t_list* tabla_paginas_nivel1 = list_create();
    t_list* tabla_paginas_nivel2 = list_create();


    for (int i = 0; i < cant_entradas_x_tabla; i++) {
        if(indice_pagina >= cant_paginas) break;
        uint32_t* puntero_indice_n2 = malloc(sizeof(uint32_t));
        *puntero_indice_n2 = indice_pagina;
        

		for (int j = 0; j < cant_entradas_x_tabla; j++){
            if(indice_pagina >= cant_paginas) break;
			t_pagina_nivel2* pagina = malloc(sizeof(t_pagina_nivel2));
            pagina->nro_pagina = indice_pagina;
			pagina->marco = 0;
            pagina->bit_presencia = 0;
            pagina->bit_uso = 0;
            pagina->bit_modificado = 0;

			list_add(tabla_paginas_nivel2, pagina);

			indice_pagina++;
		}

		list_add(tabla_paginas_nivel1, puntero_indice_n2);
	}

    loggear_trace("La tabla de primer nivel tiene [%zu] entradas", list_size(tabla_paginas_nivel1));
    loggear_trace("La tabla de segundo nivel tiene [%zu] entradas", list_size(tabla_paginas_nivel2));

    int indice = list_add(list_tablas_paginas_nivel1, tabla_paginas_nivel1);

    list_add(list_tablas_paginas_nivel2, tabla_paginas_nivel2);
    int* ppid = malloc(sizeof(uint32_t));
    *ppid = pid;
    list_add(indice_x_pid, ppid);

    char* path = string_from_format("/home/utnso/swap/%zu.swap", pid);
    printf("\n%s\n", path);
    FILE* archivoswap = fopen(path, "w");
    if(archivoswap != NULL) loggear_trace("Cree el archivo [%s]", path);
    else  loggear_error("No existe carpeta de SWAP, creala master!");

    fclose(archivoswap);
    free(path);
    return indice;
}

int sacar_de_memoria(int indice, bool cierre){
    //Poner todos sus paginas con bit_presencia = 0
    t_list* tabla_paginas_nivel2 = list_get(list_tablas_paginas_nivel2, indice);

    bool _sacar_de_memoria(t_pagina_nivel2* pagina){
        if(pagina->bit_presencia == 1){
            loggear_debug("MARCO: %zu, NRO. PAGINA: %zu",pagina->marco, pagina->nro_pagina);
            if(string_equals_ignore_case(get_algoritmo_reemplazo(), "CLOCK") || (pagina->bit_modificado && !cierre)){
                guardar_en_swap(indice, pagina->nro_pagina, pagina->marco);
            }
            
            pagina->bit_presencia = 0;
            pagina->bit_modificado = 0;
        }

        return 1;
    }

    t_list* lista_mappeada =  list_map(tabla_paginas_nivel2, (void*) _sacar_de_memoria);
    list_destroy(lista_mappeada);

    //Marcas sus marcos como libres
    bool _es_del_proceso(t_info_marco* info) {
        return ((info->indice == indice) && info->ocupado);
    }
    t_list* marcos_en_memoria = list_filter(ctrl_memoria, (void*) _es_del_proceso);

    bool _desocupar(t_info_marco* info){
        info->ocupado = 0;
        loggear_trace("Se libero el marco %zu", info->nro_marco);
        return 1;
    }

    t_list* lista_mappeada2 = list_map(marcos_en_memoria, (void*) _desocupar);
    list_destroy(lista_mappeada2);

    list_destroy(marcos_en_memoria);

    return 1;
}

int cerrar_proceso(int indice){
    if(sacar_de_memoria(indice, true)<0)return -1;
    if(borrar_swap(indice)<0)return -2;
    return 1;
}