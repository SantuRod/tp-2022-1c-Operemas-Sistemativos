#include "conexion_cpu.h"

//Funciones Privadas
int iniciar_conexion_cpu();
int recibir_mensaje_cpu(int socket_cpu);
void avisar_cpu_desconexion();


// Publica
void cpu_cerrar_conexiones(bool safe_close) {
	avisar_cpu_desconexion();

	loggear_trace("Cerrado los threads y sockets");
}

// Publica
int iniciar_conexion_cpu() {
	char * cpu_ip = get_ip_cpu();
	int cpu_puerto = get_puerto_cpu();

	int socket_server = conectar_a_servidor(cpu_ip, cpu_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de cpu DISPATCH, Ip: %s, Puerto: %d, Error: %d", cpu_ip, cpu_puerto, socket_server);

		return socket_server;
	}

	return socket_server;
}

void avisar_cpu_desconexion() {
	int socket_avisar = iniciar_conexion_cpu();

	enviar_mensaje_protocolo(socket_avisar, DESCONEXION_KERNEL_K_CPU, 0, NULL);

	close(socket_avisar);
}

int iniciar_conexion_cpu_interrupt() {
	char * cpu_ip = get_ip_cpu();
	int cpu_puerto = get_puerto_cpu_interrupt();

	int socket_server = conectar_a_servidor(cpu_ip, cpu_puerto);
	if (socket_server < 0) {
		loggear_error("Ocurrió un error al conectarse al servidor de cpu INTERRUPT, Ip: %s, Puerto: %d, Error: %d", cpu_ip, cpu_puerto, socket_server);

		return socket_server;
	}

	return socket_server;
}