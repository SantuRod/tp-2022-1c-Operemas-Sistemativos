#ifndef CONEXION_CPU_H_
#define CONEXION_CPU_H_

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "../configuracion/kernel_configuracion.h"

#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/logger.h"
#include "../../../shared/estructura_compartida.h"

/**
 * @NAME: cpu_enviar_handshake
 * @DESC: Envia un handshake a cpu para verificar la conexion
 * @RETURN:
 * 	0 salio bien
 * 	X Error, no podemos conectar
 */
int cpu_enviar_handshake();

/**
 * @NAME: cpu_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 * @PARAM:
 * safe_close = true, hace pthread_join
 * safe_close = false, hace pthread_detach
 */
void cpu_cerrar_conexiones(bool safe_close);

/**
 * @NAME: iniciar_conexion_cpu
 * @DESC: Crea un socket para conextarse con el cpu id
 * @PARAM:
 * @RETURN: socket (el socket de la conexion con el cpu)
 */
int iniciar_conexion_cpu();


/**
 * @NAME: avisar_cpu_desconexion
 * @DESC: Avisa a la cpu que el kernel se va a deconectar 
 */
void avisar_cpu_desconexion();


/**
 * @NAME: iniciar_conexion_cpu
 * @DESC: Crea un socket para conextarse con el cpu int
 * @PARAM:
 * @RETURN: socket (el socket de la conexion con el cpu)
 */
int iniciar_conexion_cpu_interrupt();
#endif /* CONEXION_CPU_H_ */
