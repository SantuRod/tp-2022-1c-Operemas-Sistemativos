/** Algoritmo de reemplazo
 * 
 * 1) Pego una vuelta entera, busco (0;0)
 * 2) Pego otra vuelta entera, busco (0;1), modifico bit uso
 * 3) Repito Paso 1
 * 4) Repito Paso 2
 * 
*/

#include "clock_m.h"

void loggear_bits(t_list* paginas_en_marco){
    for(int g = 0; g < list_size(paginas_en_marco); g++ ){
        t_pagina_nivel2* pagina_log = list_get(paginas_en_marco, g);
        loggear_debug("Marco [%zu], Bit uso [%zu], Bit modificado [%zu]", pagina_log->marco, pagina_log->bit_uso, pagina_log->bit_modificado);
    }
} 

void aumentar_clock_m(){
    if(++indice_clock == cant_marcos_x_proceso) indice_clock = 0;
    loggear_debug("Se actualiza el indice del clock-m[%zu]", indice_clock);
}

bool _esta_en_memoria_m(t_pagina_nivel2* pagina){
    return pagina->bit_presencia;
}

bool _ordenar_por_marco_m(t_pagina_nivel2 *marco_menor, t_pagina_nivel2 *marco_mayor){
    return marco_menor->marco <= marco_mayor->marco;
}

int reemplazar_clock_m(uint32_t indice, int pagina){
    bool reemplazada = false;
    int marco_usado = -1;
    t_list* tabla_paginas_nivel2 = list_get(list_tablas_paginas_nivel2, indice);
    t_list* paginas_en_marco = list_filter(tabla_paginas_nivel2, (void*) _esta_en_memoria_m);


    list_sort(paginas_en_marco, (void*) _ordenar_por_marco_m);

    loggear_debug("hay [%zu] marcos de este proceso en memoria ppal", list_size(paginas_en_marco));
    loggear_debug("Se busca un marco para reemplazar y meter la pagina");
    while(!reemplazada){
        //Paso 1 busco (0;0)
        for(int g = 0; g < list_size(paginas_en_marco); g++ ){
            loggear_info("Indice Clock [%zu]", indice_clock);
            loggear_bits(paginas_en_marco);
            t_pagina_nivel2* pagina_evaluada = list_get(paginas_en_marco, indice_clock);
            if(!pagina_evaluada->bit_uso && !pagina_evaluada->bit_modificado){
                loggear_debug("Marco Abandonado y sin modificar, se lo va a reemplazar");
                marco_usado = pagina_evaluada->marco;
                pagina_evaluada->bit_presencia = 0;
                cargar_pagina_en_memoria(indice, pagina, marco_usado);    
                reemplazada = true;  
                aumentar_clock_m();
                list_destroy(paginas_en_marco);
                return marco_usado;      
            }

            aumentar_clock_m();
        }

        //Paso 2 busco (0;1) y Modifico bit uso
        for(int g = 0; g < list_size(paginas_en_marco); g++ ){
            loggear_info("Indice Clock [%zu]", indice_clock);
            loggear_bits(paginas_en_marco);
            t_pagina_nivel2* pagina_evaluada = list_get(paginas_en_marco, indice_clock);
            if(pagina_evaluada->bit_uso){
                loggear_debug("Marco en uso, se lo marca como Abandonado");
                pagina_evaluada->bit_uso = 0;
            } else {
                marco_usado = pagina_evaluada->marco;
                pagina_evaluada->bit_presencia = 0;
                guardar_en_swap(indice, pagina_evaluada->nro_pagina , pagina_evaluada->marco);
                loggear_debug("Marco Abandonado y modificado, se lo va a reemplazar");
                    
                cargar_pagina_en_memoria(indice, pagina, marco_usado);    
                reemplazada = true;        
                aumentar_clock_m(); 
                list_destroy(paginas_en_marco);
                return marco_usado;
            }

            aumentar_clock_m(); 
        }

    }
    
    list_destroy(paginas_en_marco);
    return -1;
}