#include "manejo_memoria.h"

uint32_t leer_memoria(int direccion_fisica){  
    size_t offset = direccion_fisica; 
    if(direccion_fisica >= tam_memoria) {
        loggear_error("La direccion leida esta fuera del espacio de memoria");
        return -1;
    }
    loggear_trace("Se lee la direccion fisica [%zu] ", direccion_fisica);
    usleep(get_retardo_memoria()*1000);


    uint32_t data;
    memcpy(&data, memoria + offset, sizeof(uint32_t));


    return data;
} 

bool escribir_memoria(int direccion_fisica, uint32_t valor){ 
    size_t offset = direccion_fisica;
    if(direccion_fisica >= tam_memoria) {
        loggear_error("La direccion en la que se quiere escribir esta fuera del espacio de memoria");
        return -1;
    }
    usleep(get_retardo_memoria()*1000);
    memcpy(memoria + offset, &valor, sizeof(uint32_t));

    if(string_equals_ignore_case(get_algoritmo_reemplazo(), "CLOCK-M")){
        int marco_modificado = (direccion_fisica/tam_pagina);

        t_info_marco* info_marco = list_get(ctrl_memoria, marco_modificado);
        t_list* tabla_paginas_nivel2 = list_get(list_tablas_paginas_nivel2, info_marco->indice);

        bool _es_la_pagina_modificada(t_pagina_nivel2* pagina) {
            return ((pagina->marco == marco_modificado) && pagina->bit_presencia);
        }


        t_pagina_nivel2* pagina_modificada = list_find(tabla_paginas_nivel2, (void*) _es_la_pagina_modificada);
        loggear_trace("Se pone el bit de modificado de la pagina en 1");
        pagina_modificada->bit_modificado = 1;
    }

    return true;
} 


void loggear_memoria(){
    int direccion;
    loggear_trace("************* Loggeando Memoria *************");
    for(int i=0; i<cant_total_marcos; i++){
        loggear_trace("------- Loggeando info del marco [%zu] -------", i);
        for(int j=0; j<(tam_pagina/sizeof(uint32_t)); j++){
            direccion = i * tam_pagina + j*sizeof(uint32_t);
            uint32_t info = leer_memoria(direccion);
            loggear_trace("Loggeando info del uint32_t en posicion [%zu]: [%zu]", direccion, info);
        }
    }
    loggear_trace("**************** Fin Memoria ****************");
}

void randomizar_memoria(){
    int direccion ;
    loggear_trace("************* randomizando Memoria *************");
    for(int i=0; i<cant_total_marcos; i++){
        loggear_trace("------- randomizando info del marco [%zu] -------", i);
        for(int j=0; j<(tam_pagina/sizeof(uint32_t)); j++){

            direccion = i * tam_pagina + j*sizeof(uint32_t);
            uint32_t valor = rand()%100;
            escribir_memoria(direccion, valor);
            loggear_trace("randomizando info del uint32_t en posicion [%zu]: [%zu]", direccion, valor);
        }
    }
    loggear_trace("**************** Fin Memoria ****************");
}

void* leer_pagina_en_memoria(int marco){
    
    size_t offset = tam_pagina * marco;

    void* pagina = malloc(tam_pagina);
    memcpy(pagina, memoria + offset, tam_pagina);
    
    loggear_warning("El marco [%zu] tiene el valor [%zu]", marco, *(uint32_t*) pagina);
    
    return pagina;
}

int meter_pagina_en_memoria(void* pagina, int marco){
    
    size_t offset = tam_pagina * marco;
    memcpy(memoria + offset, pagina, tam_pagina);

    return true;
}

int cargar_pagina_en_memoria(int indice, int nro_pagina, int marco){
    loggear_warning("se va a cargar la pagina [%zu] del proceso de indice [%zu] en el marco [%zu] ", nro_pagina, indice, marco);
   
    uint32_t* pid = list_get(indice_x_pid, indice);
    
    char* path = string_from_format("/home/utnso/swap/%zu.swap", *pid);
    loggear_warning("%s", path);
    FILE* archivoswap = fopen(path, "r");
    void* pagina = malloc(tam_pagina);

    size_t offset = nro_pagina *tam_pagina;
    fseek(archivoswap, offset, SEEK_SET);
    fread(pagina, sizeof(uint32_t), tam_pagina/sizeof(uint32_t), archivoswap);

    fclose(archivoswap);
    
    meter_pagina_en_memoria(pagina, marco);

    t_info_marco* info_marco = list_get(ctrl_memoria, marco);
    info_marco->ocupado = true;
    info_marco->indice = indice;
    free(path);
    free(pagina);

    return 1;
    
}

bool _marco_libre(t_info_marco* info_marco) {
    return !(info_marco->ocupado);
}

bool hay_lugar_en_memoria(){
    bool hay_lugar = false;
    t_list* marcos_libres = list_filter(ctrl_memoria, (void*) _marco_libre);
    loggear_info("Hay [%zu] marcos libres", list_size(marcos_libres));
    if(list_size(marcos_libres)>=cant_marcos_x_proceso) hay_lugar = true;
    else hay_lugar = false;
    list_destroy(marcos_libres);
    return hay_lugar;
}

int reservar_primer_marco_libre(int indice){
    for(int i = 0; i < list_size(ctrl_memoria); i++){
        t_info_marco* info_marco = list_get(ctrl_memoria, i);
        if(!info_marco->ocupado){
            info_marco->ocupado = true;
            info_marco->indice = indice;
            return info_marco->nro_marco;
        }
    }

    return -1; 
}
int meter_proceso_en_memoria(int indice){
    loggear_trace("se va a pasar a memoria el proceso [%zu] con indice", indice);

    if(!hay_lugar_en_memoria()){
        loggear_error("No hay marcos libres para meter el proceso en memoria, se lo rechaza");
        return -1;
    }

    loggear_trace("Se le asignan [%zu] marcos", cant_marcos_x_proceso);
    for(int i = 0; i < cant_marcos_x_proceso; i++){
        int marco = reservar_primer_marco_libre(indice);
        loggear_info("se encontro el marco [%zu] libre", marco);
    }

    return 0;
}

void guardar_en_swap(int indice, int nro_pagina , int marco){
    loggear_warning("Se escribe swap");
    usleep(get_retardo_swap()*1000);
    void* pagina = leer_pagina_en_memoria(marco);

    uint32_t* pid = list_get(indice_x_pid, indice);
    
    char* path = string_from_format("/home/utnso/swap/%zu.swap", *pid);
    FILE* archivoswap = fopen(path, "r+");
        
    size_t offset = nro_pagina *tam_pagina;
    loggear_warning("se abrio el archivo %s en modo r+", path);
    loggear_warning("se escribio la pagina [%zu] en el archivo", nro_pagina);
    fseek(archivoswap, offset, SEEK_SET);
    fwrite(pagina, sizeof(uint32_t), tam_pagina/sizeof(uint32_t), archivoswap);
    free(pagina);
    free(path);
    fclose(archivoswap);
}

void leer_swap(int direccion, int indice){
    loggear_warning("Se lee swap");
    usleep(get_retardo_swap()*1000);
    uint32_t* pid = list_get(indice_x_pid, indice);
    
    char* path = string_from_format("/home/utnso/swap/%zu.swap", *pid);
    FILE* archivoswap = fopen(path, "r");
    void* pagina = malloc(tam_pagina);

    fseek(archivoswap, direccion, SEEK_SET);
    fread(pagina, sizeof(uint32_t), tam_pagina/sizeof(uint32_t), archivoswap);
    loggear_warning("se leyo el valor [%zu] en el archivo", *(uint32_t*) pagina);
    free(pagina);
    free(path);
    fclose(archivoswap);
}

int borrar_swap(int indice){
    uint32_t* pid = list_get(indice_x_pid, indice);
    
    char* path = string_from_format("/home/utnso/swap/%zu.swap", *pid);

    remove(path);
    free(path);
    return 1;
}

void loggear_ctrl_memoria(){
    for(int i = 0; i < list_size(ctrl_memoria); i++){
        t_info_marco* info = list_get(ctrl_memoria, i);
        loggear_debug("marco [%zu], proceso [%zu] y ocupado [%zu]",info->nro_marco , info->indice, info->ocupado);
    }
}