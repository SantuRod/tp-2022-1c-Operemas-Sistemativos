
#ifndef COORDINAR_HILOS_H_
#define COORDINAR_HILOS_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"

#include "manejar_mensajes_kernel.h"

sem_t semaforo_detener_ejecucion;
/**
 * @NAME: conectar_kernel
 * @DESC: Bloquea el hilo en donde se ejecute para escuchar conexiones del kernel y manejar mensajes.
 * Solo se va a detener cuando maten el hilo desde donde se invoca o el kernel se desconecte.
 */
int conectar_kernel();

#endif /* COORDINAR_HILOS_H_ */
