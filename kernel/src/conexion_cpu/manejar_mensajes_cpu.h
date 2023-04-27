
#ifndef MANEJAR_MENSAJES_CPU_H_
#define MANEJAR_MENSAJES_CPU_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/estructura_compartida.h"
#include "conexion_cpu.h"



/**
 * @NAME: recibir_mensaje_cpu
 * @DESC: Espera un mensaje de cpu, actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensajes_cpu"
 */
int recibir_mensaje_cpu(int socket_cpu);

#endif /* MANEJAR_MENSAJES_CPU_H_ */
