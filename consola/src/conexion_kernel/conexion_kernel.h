#ifndef CONEXION_KERNEL_H_
#define CONEXION_KERNEL_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../configuracion/consola_configuracion.h"

#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"
#include "../../../shared/estructura_compartida.h"

/**
 * @NAME: kernel_enviar_handshake
 * @DESC: Envia un handshake a kernel para verificar la conexion
 * @RETURN:
 * 	0 salio bien
 * 	X Error, no podemos conectar
 */
int kernel_enviar_handshake();

/**
 * @NAME: kernel_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 * @PARAM:
 * safe_close = true, hace pthread_join
 * safe_close = false, hace pthread_detach
 */
void kernel_cerrar_conexiones(bool safe_close);

/**
 * @NAME: iniciar_conexion_kernel
 * @DESC: Crea un socket para conextarse con el kernel
 * @PARAM:
 * @RETURN: socket (el socket de la conexion con el kernel)
 */
int iniciar_conexion_kernel();

void avisar_kernel_desconexion();
#endif /* CONEXION_KERNEL_H_ */
