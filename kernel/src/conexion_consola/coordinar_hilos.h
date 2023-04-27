
#ifndef COORDINAR_HILOS_H_
#define COORDINAR_HILOS_H_

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"

#include "manejar_mensajes_consola.h"


/**
 * @NAME: conectar_consola
 * @DESC: Bloquea el hilo en donde se ejecute para escuchar conexiones del discordiador y manejar mensajes.
 * Solo se va a detener cuando maten el hilo desde donde se invoca o el discordiador se desconecte.
 */
int conectar_consola();
bool continuar_escuchando;
#endif /* COORDINAR_HILOS_H_ */
