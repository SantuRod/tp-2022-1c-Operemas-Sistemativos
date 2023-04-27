
#ifndef MANEJAR_MENSAJES_KERNEL_H_
#define MANEJAR_MENSAJES_KERNEL_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/estructura_compartida.h"
#include "conexion_kernel.h"



/**
 * @NAME: recibir_mensaje_kernel
 * @DESC: Espera un mensaje de kernel, actua segun corresponda.
 * @RETURN:
 * Llama a "manejar_mensajes_kernel"
 */
int recibir_mensaje_kernel(int socket_kernel);
int manejar_mensajes_kernel(t_prot_mensaje * mensaje);

void* serializarInstruccionesConsola (t_list* instrucciones, uint32_t tam_consola, size_t* tam_paquete);
#endif /* MANEJAR_MENSAJES_KERNEL_H_ */
