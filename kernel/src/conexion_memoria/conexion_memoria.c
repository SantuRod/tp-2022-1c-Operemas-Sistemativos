#include "conexion_memoria.h"

//Funciones Privadas
int iniciar_conexion_memoria();
int recibir_mensaje_memoria(int socket_memoria);
void avisar_memoria_desconexion();


// Publica
void memoria_cerrar_conexiones(bool safe_close) {
	avisar_memoria_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

// Publica
int iniciar_conexion_memoria() {
	char * memoria_ip = get_ip_memoria();
	int memoria_puerto = get_puerto_memoria();

	int socket_server = conectar_a_servidor(memoria_ip, memoria_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de memoria, Ip: %s, Puerto: %d, Error: %d", memoria_ip, memoria_puerto, socket_server);

		return socket_server;
	}

	return socket_server;
}

void avisar_memoria_desconexion() {
	int socket_avisar = iniciar_conexion_memoria();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_KERNEL_K_M, 0, NULL);

	close(socket_avisar);
}
