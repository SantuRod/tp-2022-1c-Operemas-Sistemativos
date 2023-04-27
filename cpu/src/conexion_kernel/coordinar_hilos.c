
#include "coordinar_hilos.h"

pthread_mutex_t * mutex_threads;

int conectar_kernel(void * socket_servidor) {
	int socket = *((int *) socket_servidor);

	bool continuar_escuchando = true;

	while ( continuar_escuchando ) {
		int socket_kernel = aceptar_conexion_protocolo(socket);
		if (socket_kernel < 0) {
			loggear_error("Ocurrio un error al aceptar la conexion: %d", socket_kernel);
			continue;
		}
		t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_kernel);
		loggear_warning("Recibo un mensaje %zu", mensaje->head);

		if (mensaje->head == DESCONEXION_KERNEL_K_CPU) {
			loggear_trace("Se desconectara el kernel");
			loggear_trace("Aviso a memoria que se desconecte");
			avisar_memoria_desconexion();
			continuar_escuchando = false;
			destruir_mensaje(mensaje);
			break;
		}

		pthread_attr_t tattr;
		pthread_t tid;
		int ret;

		ret = pthread_attr_init(&tattr);
		ret = pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
		ret = pthread_create(&tid, &tattr, (void *) manejar_mensaje_kernel, mensaje);

		if (ret != 0) {
			loggear_error("Ocurrió un error al crear el hilo para escuchar el mensaje, Error: %d", ret);
			if (ret == 11) {
				loggear_error("No se pueden crear mas threads, error %d", ret);
			}
		}

		loggear_warning("Se creo el hilo [%zu] pendiente para destruir", ret);
	}
	
	return 0;
}
