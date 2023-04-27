#include "conexion_kernel.h"


// Publica
void kernel_cerrar_conexiones(bool safe_close) {
	avisar_kernel_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

// Publica
int iniciar_conexion_kernel() {
	char * kernel_ip = get_ip_kernel();
	int kernel_puerto = get_puerto_kernel();

	int socket_server = conectar_a_servidor(kernel_ip, kernel_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de kernel, Ip: %s, Puerto: %d, Error: %d", kernel_ip, kernel_puerto, socket_server);

		return socket_server;
	}

	return socket_server;
}

void avisar_kernel_desconexion() {
	int socket_avisar = iniciar_conexion_kernel();

	if(enviar_mensaje_protocolo(socket_avisar, DESCONEXION_CONSOLA_CO_K, 0, NULL)<0) loggear_error("Ocurrió un error al conectarse al servidor de kernel.");
	if(close(socket_avisar)==0) loggear_info("Se cerro la conexion exitosamente");
	
}