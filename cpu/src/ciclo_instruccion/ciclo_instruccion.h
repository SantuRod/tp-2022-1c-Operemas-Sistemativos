
#ifndef CICLO_INSTRUCCION_H_
#define CICLO_INSTRUCCION_H_

#include <commons/collections/list.h>

#include <sys/mman.h>

#include "../configuracion/cpu_configuracion.h"	// Carga del archivo de configuracion
#include <readline/readline.h>
#include <signal.h>

#include "../conexion_kernel/conexion_kernel.h"
#include "../conexion_kernel/coordinar_hilos.h"
#include "../conexion_kernel/manejar_mensajes_kernel.h"
#include "../mmu/mmu.h"

#include "../../../shared/logger.h"			        // Mutex para escribir en consola y en archivo de log
#include "../../../shared/conexion.h"			    // Crear y conectarse a servidor
#include "../../../shared/protocolo.h"			    // Enviar mensajes a través de sockets
#include "../../../shared/manejo_hilos.h"		    // Manejar cierre de hilos y loggear en caso de error
#include "../../../shared/estructura_compartida.h"  // Estructuras de structs compartidos por todos los módulos

typedef enum {
    SIGUIENTE_INSTRUCCION,
    DESALOJO_INTERRUPT,
    DESALOJO_IO,
    DESALOJO_EXIT
} t_id_desalojo;
pthread_mutex_t m_interrupt;
void iniciar_mutex();
void set_interrupcion_kernel_true();
void set_interrupcion_kernel_false();
int get_ciclosIO();
void ejecutar_ciclo_instruccion(t_pcb * pcb);
t_instruccion * fetch(t_pcb * pcb);
bool decode(t_instruccion * instruccion);
uint32_t fetch_operands(uint32_t dir_logica, uint32_t tabla_paginas);
t_id_desalojo execute(t_instruccion * instruccion, uint32_t valor_en_memoria, t_pcb * pcb);
bool check_interrupt();
void desalojar_PCB(t_pcb * pcb, t_id_desalojo id_desalojo);
void destructor_PCB(t_pcb* pcb);


#endif /* CICLO_INSTRUCCION_H_ */