
#ifndef CONEXION_CPU_Y_KERNEL_H_
#define CONEXION_CPU_Y_KERNEL_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <commons/collections/queue.h>

#include "../configuracion/memoria_configuracion.h"
#include "../configuracion/memoria_config_guardada.h"

#include "../../../shared/logger.h"
#include "../../../shared/conexion.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"

#include "manejar_mensajes_cpu_y_kernel.h"

/**
 * @NAME: levantar_servidor
 * @DESC: Crea un hilo para esperar la conexión de la consola
 * Al guardar el socket correctamente, se auto-destruye el hilo.
 */
int levantar_servidor();

/**
 * @NAME: cerrar_conexiones
 * @DESC: Cierra todos los sockets y libera la memoria de las variables. Llamar siempre
 */
void cerrar_conexiones(bool safe_close);

#endif /* CONEXION_CPU_Y_KERNEL_H_ */
