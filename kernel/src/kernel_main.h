
#ifndef KERNEL_MAIN_H_
#define KERNEL_MAIN_H_

#include <commons/collections/list.h>

#include <sys/mman.h>

#include "configuracion/kernel_configuracion.h"
#include "configuracion/kernel_configuracion_guardada.h"
#include <math.h>
#include <signal.h>

#include "../../shared/logger.h"
#include "../../shared/conexion.h"
#include "../../shared/protocolo.h"

#include "./conexion_consola/conexion_consola.h"
#include "./conexion_consola/coordinar_hilos.h"
#include "manejo_planificador.h"
#include "kernel_global.h"


void inicializar_planificadores();
void destructorPCB(t_pcb* pcb);
void destroy_planificadores();
void destruirInstruccion(void* instruccion);

sem_t mutex_debug;

#endif /* KERNEL_MAIN_H_ */
