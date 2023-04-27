#ifndef CONEXION_MEMORIA_H_
#define CONEXION_MEMORIA_H_

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
 * @NAME: memoria_cerrar_conexiones
 * @DESC: Cierra los hilos y libera toda la memoria ocupada
 * @PARAM:
 * safe_close = true, hace pthread_join
 * safe_close = false, hace pthread_detach
 */
void memoria_cerrar_conexiones(bool safe_close);

/**
 * @NAME: iniciar_conexion_memoria
 * @DESC: Crea un socket para conextarse con el memoria
 * @PARAM:
 * @RETURN: socket (el socket de la conexion con el memoria)
 */
int iniciar_conexion_memoria();

#endif /* CONEXION_MEMORIA_H_ */
