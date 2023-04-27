#include "mmu.h"

// Este modulo va a estar muy relacionado con conexion_memoria 
//Tiro un par de conversiones de las direcciones

//El objetivo es obtener las direcciones lógicas propias del proceso a direcciones físicas de la memoria

uint32_t pcb_id = 0;

// TLB GENERAL PARA MMU
t_list * tlb;


uint32_t checkear_algoritmo_reemplazo(){
    return strcmp(config_guardada.reemplazo_tlb,"FIFO"); //Si son iguales, devuelve 0.
}

/*------------------------------------------MMU-------------------------------------------*/

uint32_t peticion_READ_MMU(uint32_t dir_logica, uint32_t tabla_paginas){
    loggear_info("Entro a la MMU para hacer READ");
    uint32_t valor_leido = peticion_memoria(dir_logica, tabla_paginas, READ_OPER,0); // 0 READ; 1 WRITE
    return valor_leido;
}

void peticion_WRITE_MMU(uint32_t dir_logica, uint32_t tabla_paginas, uint32_t valor_a_escribir){
    loggear_info("Entro a la MMU para hacer WRITE");
    peticion_memoria(dir_logica, tabla_paginas, WRITE_OPER, valor_a_escribir); // 0 READ; 1 WRITE
    loggear_info("Write exitoso.");
}

uint32_t peticion_memoria(uint32_t dir_logica, uint32_t tabla_paginas, uint32_t tipo_operacion, uint32_t valor_a_escribir){
    uint32_t numero_pagina = calcular_numero_pagina(dir_logica);
    uint32_t entrada_tabla_1er_nivel;
    uint32_t entrada_tabla_2do_nivel;
    uint32_t desplazamiento = 0;
    uint32_t dir_fisica = 0;
    int dir_tabla_2do_nivel = 0;
    uint32_t valor_leido = 0;

    //TRADUCCIONES
    entrada_tabla_1er_nivel = calcular_entrada_tabla_1er_nivel(numero_pagina);
    entrada_tabla_2do_nivel = calcular_entrada_tabla_2do_nivel(numero_pagina);
    desplazamiento = calcular_desplazamiento(dir_logica, numero_pagina);
    loggear_trace("==========================================================");
    loggear_trace("DIRECCIÓN LÓGICA: %zu", dir_logica);
    loggear_trace("NÚMERO DE PÁGINA: %zu", numero_pagina);
    loggear_trace("ENTRADA TABLA DE PRIMER NIVEL: %zu", entrada_tabla_1er_nivel);
    loggear_trace("ENTRADA TABLA DE SEGUNDO NIVEL: %zu", entrada_tabla_2do_nivel);
    loggear_trace("DESPLAZAMIENTO: %zu", desplazamiento);
    loggear_trace("==========================================================");
    //TRADUCCIONES
    loggear_info("Procedo a buscar la página %zu en la TLB", numero_pagina);
    int marco_TLB = buscar_en_TLB(numero_pagina);
    if(marco_TLB < 0)
    {
        dir_tabla_2do_nivel = pedir_direccion_tabla_nivel2(tabla_paginas,entrada_tabla_1er_nivel);
        loggear_trace("Recibimos la direccion de la tabla de 2do nivel: %zu", dir_tabla_2do_nivel);
        if (dir_tabla_2do_nivel < 0){
            return 0;
        }
        marco_TLB = pedir_marco(tabla_paginas, entrada_tabla_2do_nivel + dir_tabla_2do_nivel);
        loggear_trace("Recibimos el marco n°%zu.", marco_TLB);
        if (marco_TLB < 0){
            return 0;
        }
        dir_fisica = marco_TLB * tamano_pagina + desplazamiento;
        loggear_trace("==========================================================");
        loggear_trace("DIRECCIÓN FÍSICA: %zu", dir_fisica);
        loggear_trace("==========================================================");
        if (tipo_operacion){
            loggear_trace("Se hace petición WRITE a memoria.");
            pedir_write(dir_fisica, valor_a_escribir);
        }
        else{
            loggear_trace("Se hace petición READ a memoria.");
            valor_leido = pedir_read(dir_fisica);
        }
        agregar_a_tlb(numero_pagina, marco_TLB);
        return valor_leido;
    }
    loggear_trace("Se encontró el marco %zu. Se procede a acceder a memoria.", marco_TLB);
    dir_fisica = marco_TLB * tamano_pagina + desplazamiento;
    loggear_trace("==========================================================");
    loggear_trace("DIRECCIÓN FÍSICA: %zu", dir_fisica);
    loggear_trace("==========================================================");
    if (tipo_operacion){
        loggear_trace("Se hace petición WRITE a memoria.");
        pedir_write(dir_fisica, valor_a_escribir);
    }
    else{
        loggear_trace("Se hace petición READ a memoria.");
        valor_leido = pedir_read(dir_fisica);
    }
    return valor_leido;  
}

/*
número_página = floor(dirección_lógica / tamaño_página)
entrada_tabla_1er_nivel = floor(número_página / cant_entradas_por_tabla)
entrada_tabla_2do_nivel = número_página mod (cant_entradas_por_tabla)
desplazamiento = dirección_lógica - número_página * tamaño_página
*/

/*--------------------------------------TRADUCCIONES--------------------------------------*/
uint32_t calcular_numero_pagina(uint32_t dir_logica)
{
    return dir_logica / tamano_pagina;
}

uint32_t calcular_entrada_tabla_1er_nivel(uint32_t numero_pagina)
{
    return numero_pagina / cant_entradas_por_tabla;
}

uint32_t calcular_entrada_tabla_2do_nivel(uint32_t numero_pagina)
{
    return numero_pagina % cant_entradas_por_tabla;
}

uint32_t calcular_desplazamiento(uint32_t dir_logica, uint32_t numero_pagina)
{
    return dir_logica - numero_pagina * tamano_pagina;
}

/*--------------------------------------TLB--------------------------------------*/


void iniciar_TLB(){
    tlb = list_create();
}

int buscar_en_TLB(uint32_t numero_pagina)
{
    bool matchea_pagina(t_tlb * valores_tlb)
    {
        return valores_tlb->pagina == numero_pagina;
    }
    t_tlb *  tlb_elem;
    loggear_trace("Busco dentro de la TLB");
    tlb_elem = list_find(tlb,(void *)matchea_pagina);
    if (tlb_elem == NULL)
    {
    loggear_warning("=========================================================");
    loggear_warning("|                    !!!TLB MISS!!!                     |");
    loggear_warning("=========================================================");
        return -1;
    }
    loggear_debug("=========================================================");
    loggear_debug("|                     !!!TLB HIT!!!                     |");
    loggear_debug("=========================================================");
    if (checkear_algoritmo_reemplazo()){
        loggear_info("Como el algoritmo es LRU, debo ordenar la lista para que sea el más recientemente usado.");
        ordenar_tlb(tlb_elem, numero_pagina);
    }

    return tlb_elem->marco;
}

void agregar_a_tlb(uint32_t numero_pagina, uint32_t marco){
    t_tlb * registro_tlb = malloc(sizeof(t_tlb));
    registro_tlb->pagina = numero_pagina;
    registro_tlb->marco = marco;
    bool matchea_marco(t_tlb * valores_tlb)
    {
        return valores_tlb->marco == marco;
    }
    loggear_trace("La TLB se prepara para guardar [P%zu|M%zu]", registro_tlb->pagina, registro_tlb->marco);
    if (list_find(tlb,(void *) matchea_marco)){
        loggear_warning("Se tenía una referencia antigua a este marco. Se elimina de la TLB.");
        list_remove_and_destroy_by_condition(tlb,(void *) matchea_marco, free);
    }
    if (list_size(tlb) >= config_guardada.entradas_tlb)
    {
        loggear_warning("La TLB está llena. Se debe eliminar el último registro.");
        list_remove_and_destroy_element(tlb,0,free);
    }
    loggear_trace("Se eliminó con éxito.");
    list_add(tlb, registro_tlb);
    loggear_info("Se agregó el registro con éxito. La TLB tiene %zu elemento/s.", list_size(tlb));
}

void ordenar_tlb(t_tlb * tlb_elem, uint32_t numero_pagina){
    bool matchea_pagina(t_tlb * valores_tlb)
    {
        return valores_tlb->pagina == numero_pagina;
    }
    loggear_info("Se procede a ordenar la TLB.");
    list_remove_by_condition(tlb,(void *)matchea_pagina);
    list_add(tlb, tlb_elem);
}

void limpiar_TLB(uint32_t pcb_id_ciclo){
    if (pcb_id_ciclo != pcb_id){
        loggear_warning("Ocurrió un cambio de proceso. Se procede a limpiar la TLB.");
        list_clean_and_destroy_elements(tlb,free);
        pcb_id = pcb_id_ciclo;
        loggear_info("TLB limpiada con éxito.");
    }
    else{
        loggear_trace("Llegó el mismo proceso. La TLB se mantiene.");
    }
}

void destruir_TLB(){
    list_destroy_and_destroy_elements(tlb, free);
}