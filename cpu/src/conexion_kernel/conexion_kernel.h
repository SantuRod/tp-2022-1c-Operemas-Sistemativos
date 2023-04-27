
#ifndef CONEXION_KERNEL_H_
#define CONEXION_KERNEL_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <commons/collections/queue.h>

#include "../configuracion/cpu_configuracion.h"
#include "../configuracion/cpu_configuracion_guardada.h"

#include "../../../shared/logger.h"
#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"

#include "manejar_mensajes_kernel.h"

/**
 * @NAME: levantar_servidor
 * @DESC: Crea un hilo para esperar la conexión del kernel,
 * Al guardar el socket correctamente, se auto-destruye el hilo.
 */
int levantar_servidor();

/**
 * @NAME: cerrar_conexiones
 * @DESC: Cierra todos los sockets y libera la memoria de las variables. Llamar siempre
 */
void cerrar_conexiones(bool safe_close);

#endif /* CONEXION_KERNEL_H_ */
