#include "conexion_kernel.h"

//Funciones Privadas
int conectar_kernel();
int prender_servidor();


int * socket_dispatch; // Socket del dispatch donde se conectan
int * socket_interrupt; // Socket de interrupt donde cpu recibe mensajes de interrupcion

pthread_t thread_escuchar_notificaciones; // Donde está el hilo que crea hilos por conexiones

// Publica, Hilos: 1, Crea: 1
int levantar_servidor() {
	int server_error = prender_servidor();
	if (server_error < 0) {
		loggear_error("Ocurrio un error al iniciar el servidor");
		return server_error;
	}

	int error = pthread_create(&thread_escuchar_notificaciones, NULL, (void *) conectar_kernel, (void *) socket_dispatch);
	if (error != 0) {
		loggear_error("Ocurrió un error al crear el hilo de dispatch, Error: %d", error);
		return error;
	}

	error = pthread_create(&thread_escuchar_notificaciones, NULL, (void *) conectar_kernel, (void *) socket_interrupt);
	if (error != 0) {
		loggear_error("Ocurrió un error al crear el hilo de interrupt, Error: %d", error);
		return error;
	}

	loggear_trace("Creado el servidor");

	return 0;
}


// Publica
void cerrar_conexiones(bool safe_close) {

	if (safe_close) {
		pthread_join(thread_escuchar_notificaciones, NULL);
	} else {
		pthread_detach(thread_escuchar_notificaciones);
	}

	loggear_trace("Cerramos todos los threads del consola que no hayan terminado");

	if (*socket_dispatch > 0) {
		close(*socket_dispatch);
	}

	if (*socket_interrupt > 0){
		close(*socket_interrupt);
	}

	free(socket_dispatch);
	free(socket_interrupt);

	loggear_trace("Cerrado los threads y sockets");
}

int prender_servidor() {
	socket_dispatch = malloc(sizeof(int));
	socket_interrupt = malloc(sizeof(int));
	*socket_dispatch = crear_servidor(get_puerto_escucha_dispatch());
	*socket_interrupt = crear_servidor(get_puerto_escucha_interrupt());

	if (*socket_dispatch < 0) {
		loggear_error("Ocurrio un error al crear el servidor: %d", *socket_dispatch);
		return ERROR_AL_ABRIR_SOCKET;
	}

	if (*socket_interrupt < 0) {
		loggear_error("Ocurrio un error al crear el servidor: %d", *socket_interrupt);
		return ERROR_AL_ABRIR_SOCKET;
	}

	return 0;
}

