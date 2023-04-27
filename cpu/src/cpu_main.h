
#ifndef CPU_MAIN_H_
#define CPU_MAIN_H_

#include <commons/collections/list.h>

#include <sys/mman.h>

#include "./configuracion/cpu_configuracion.h"	// Carga del archivo de configuracion
#include <readline/readline.h>
#include <signal.h>

#include "./conexion_memoria/manejar_mensajes_memoria.h"
#include "./conexion_kernel/conexion_kernel.h"
#include "./conexion_kernel/coordinar_hilos.h"
#include "./mmu/mmu.h"
#include "./ciclo_instruccion/ciclo_instruccion.h"

#include "../../shared/logger.h"			// Mutex para escribir en consola y en archivo de log
#include "../../shared/conexion.h"			// Crear y conectarse a servidor
#include "../../shared/protocolo.h"			// Enviar mensajes a travez de sockets
#include "../../shared/manejo_hilos.h"		// Manejar cierre de hilos y loggear en caso de error

#endif /* CPU_MAIN_H_ */
