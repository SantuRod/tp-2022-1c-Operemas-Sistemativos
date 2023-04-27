
#ifndef MANEJAR_MENSAJES_MEMORIA_H_
#define MANEJAR_MENSAJES_MEMORIA_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/estructura_compartida.h"
#include "conexion_memoria.h"



/**
 * @NAME: recibir_mensaje_memoria
 * @DESC: Espera un mensaje de memoria, actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensajes_memoria"
 */
int recibir_mensaje_memoria(int socket_memoria);

#endif /* MANEJAR_MENSAJES_MEMORIA_H_ */
