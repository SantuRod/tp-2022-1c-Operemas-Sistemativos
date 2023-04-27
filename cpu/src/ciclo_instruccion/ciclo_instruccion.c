#include "ciclo_instruccion.h"

t_id_desalojo id_desalojo = 0;
int ciclos_IO;
bool interrupcion_kernel = false;

void iniciar_mutex(){
    pthread_mutex_init(&m_interrupt, NULL);
	loggear_info("Se levanta semáforo para checkeo de interrupciones.");
}
void set_interrupcion_kernel_true(){
    loggear_warning("=========================================================");
    loggear_warning("|                    !!!INTERRUPT!!!                    |");
    loggear_warning("=========================================================");
    interrupcion_kernel = true;
}

void set_interrupcion_kernel_false(){
    interrupcion_kernel = false;
}

int get_ciclosIO(){
    return ciclos_IO;
}

void ejecutar_ciclo_instruccion(t_pcb *pcb)
{
    pthread_mutex_lock(&m_interrupt);
    set_interrupcion_kernel_false();
	pthread_mutex_unlock(&m_interrupt);
    loggear_trace("ID DE PCB: %zu",pcb->id);
    limpiar_TLB(pcb->id);
    t_instruccion * instruccion = fetch(pcb);
    uint32_t valor_copy = 0;
    if (decode(instruccion)){
        loggear_trace("Decode detectó COPY. Se obtiene valor de memoria.");
        valor_copy = fetch_operands(instruccion->segundoOperando, pcb->tabla_paginas);
    }
    do
    {
        loggear_info("La CPU se prepara para ejecutar la próxima instrucción.");
        if (decode(instruccion)){
            loggear_trace("Decode detectó COPY. Se obtiene valor de memoria.");
            valor_copy = fetch_operands(instruccion->segundoOperando, pcb->tabla_paginas);
        }
        id_desalojo = execute(instruccion, valor_copy, pcb);
        pcb->program_counter++;
        if (check_interrupt()){
            id_desalojo = DESALOJO_INTERRUPT;
        }
        else{
            instruccion = fetch(pcb);
            if (instruccion == NULL){
                id_desalojo = DESALOJO_EXIT;
            }
        }
    } while (id_desalojo == SIGUIENTE_INSTRUCCION);
    desalojar_PCB(pcb,id_desalojo);
}



t_instruccion * fetch(t_pcb *pcb)
{
    uint32_t list_tamano = list_size(pcb->instrucciones);
    loggear_trace("Ejecutando fetch...");
    if (pcb->program_counter < list_tamano){
        t_instruccion * instruccion = instruccion = (t_instruccion *)list_get(pcb->instrucciones, pcb->program_counter);
        return instruccion;
    }
    else{
        return NULL;
    }
}

bool decode(t_instruccion * instruccion)
{
    return instruccion->id_instruccion == COPY;
}

uint32_t fetch_operands(uint32_t dir_logica, uint32_t tabla_paginas)
{
    loggear_info("Busco el valor a copiar en MMU");
    uint32_t valor_copy = peticion_READ_MMU(dir_logica, tabla_paginas);  //El valor leido es el que se copia
    return valor_copy;
}

t_id_desalojo execute(t_instruccion * instruccion, uint32_t valor_copy, t_pcb * pcb)
{
    uint32_t dir_logica = instruccion->primerOperando;          //direcciones lógicas a usar en READ,WRITE y COPY.
    uint32_t valor_a_escribir = instruccion->segundoOperando;   // para WRITE
    uint32_t valor_leido = 0;                                   // para READ
    switch(instruccion->id_instruccion){
        case NO_OP:
            loggear_debug("==========================================================");
            loggear_debug("NO-OP");
            loggear_debug("==========================================================");
            usleep(config_guardada.retardo_noop * 1000); 
            loggear_info("La CPU terminó la ejecución de NO_OP.");
        break;
        case IO:
            ciclos_IO = instruccion->primerOperando;                    // para I/O
            loggear_debug("==========================================================");
            loggear_debug("I/O");
            loggear_debug("Ciclos I/O: %zu", ciclos_IO);
            loggear_debug("==========================================================");
            loggear_info("Envío señal de I/O a Kernel. Debe esperar %zu segundos.", ciclos_IO);
            return DESALOJO_IO;
        break;
        case READ:
            loggear_debug("==========================================================");
            loggear_debug("READ");
            loggear_debug("Dirección lógica: %zu", dir_logica);
            loggear_debug("==========================================================");
            valor_leido = peticion_READ_MMU(dir_logica, pcb->tabla_paginas);
            printf("El valor que tenía guardada la dirección lógica es: %zu\n", valor_leido);
        break;
        case WRITE:
            loggear_debug("==========================================================");
            loggear_debug("WRITE");
            loggear_debug("Dirección lógica: %zu. Valor a escribir: %zu", dir_logica, valor_a_escribir);
            loggear_debug("==========================================================");
            peticion_WRITE_MMU(dir_logica, pcb->tabla_paginas, valor_a_escribir);
        break;
        case COPY:
            loggear_debug("==========================================================");
            loggear_debug("COPY");
            loggear_debug("==========================================================");
            loggear_debug("Dirección lógica: %zu. Valor copiado: %zu", dir_logica,valor_copy);
            peticion_WRITE_MMU(dir_logica, pcb->tabla_paginas, valor_copy);
        break;
        case EXIT:
            loggear_debug("==========================================================");
            loggear_debug("EXIT");
            loggear_debug("==========================================================");
            loggear_info("El proceso finalizó. Enviando a Kernel para finalizar su ejecución.");
            return DESALOJO_EXIT;
        break;
        default:
            loggear_error("Instrucción desconocida.");
            break;
    }
    return SIGUIENTE_INSTRUCCION;
}

bool check_interrupt()
{
    return interrupcion_kernel;
}

void desalojar_PCB(t_pcb * pcb, t_id_desalojo id_desalojo){
    pthread_mutex_lock(&m_interrupt);
    set_interrupcion_kernel_false();
	pthread_mutex_unlock(&m_interrupt);
    switch (id_desalojo)
    {
        case DESALOJO_INTERRUPT:
            loggear_trace("Se desaloja por interrupt");
            ciclos_IO = -2;
            break;
        case DESALOJO_IO:
            loggear_trace("Se desaloja por I/O por %zu ciclos.", ciclos_IO);
            break;
        case DESALOJO_EXIT:
            loggear_trace("Se desaloja por finalizacion de proceso.");
            ciclos_IO = -1;
            break;
        default:
            loggear_error("Caso desconocido. Se destruye la pcb");
            break;
    }
}

void destructor_PCB(t_pcb* pcb){
	list_destroy_and_destroy_elements(pcb->instrucciones,free);
	free(pcb);
}