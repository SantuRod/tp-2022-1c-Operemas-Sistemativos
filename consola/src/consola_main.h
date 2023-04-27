
#ifndef CONSOLA_MAIN_H_
#define CONSOLA_MAIN_H_

#include <pthread.h>
#include <readline/readline.h>

#include "configuracion/consola_configuracion.h"	// Carga del archivo de configuracion


#include "../../shared/logger.h"			// Mutex para escribir en consola y en archivo de log
#include "../../shared/conexion.h"			// Crear y conectarse a servidor
#include "../../shared/protocolo.h"			// Enviar mensajes a travez de sockets
#include "../../shared/manejo_hilos.h"		// Manejar cierre de hilos y loggear en caso de error

#include "parser.h"

#include "./conexion_kernel/manejar_mensajes_kernel.h"

int flagDebug;


#endif /* CONSOLA_MAIN_H_ */
