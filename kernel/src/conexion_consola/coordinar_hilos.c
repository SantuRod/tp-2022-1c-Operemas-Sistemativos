#include "coordinar_hilos.h"

pthread_mutex_t * mutex_threads;
bool continuar_escuchando;

int conectar_consola(void * socket_servidor) {
	
	
	int socket = *((int *) socket_servidor);
	continuar_escuchando = true;

	while (continuar_escuchando) {
		loggear_info("coordinar hilos sigue escuchando");
		int socket_consola = aceptar_conexion_protocolo(socket);
		if (socket_consola < 0) {
			loggear_error("Ocurrio un error al aceptar la conexion: %d", socket_consola);
			continue;
		}
		t_prot_mensaje* mensaje = recibir_mensaje_protocolo(socket_consola);
		loggear_warning("Recibo un mensaje %zu", mensaje->head);

		if (mensaje->head == DESCONEXION_CONSOLA_CO_K) {
			loggear_trace("Se desconectara la consola");
		}

		if(mensaje!=NULL){
			int ret = manejar_mensaje_consola(mensaje);
			if (ret != 0 && ret != -22) {
				loggear_error("Ocurrió un error al escuchar el mensaje, Error: %d", ret);
				if (ret == 11) {
					loggear_error("No se pueden crear mas threads, error %d", ret);
				}
			}
		}
	}

	return 0;
}
