#include "manejar_mensajes_cpu.h"

//Funciones Privadas
int manejar_mensajes_cpu(t_prot_mensaje * mensaje);

int recibir_mensaje_cpu(int socket_cpu) {
	t_prot_mensaje * mensaje_cpu = recibir_mensaje_protocolo(socket_cpu);

	return manejar_mensajes_cpu(mensaje_cpu);
}

int manejar_mensajes_cpu(t_prot_mensaje * mensaje) {
	switch (mensaje->head) {
	case DESCONEXION_CPU_CPU_K:
	case DESCONEXION:
		loggear_warning("Se cerró la conexión con cpu");

		destruir_mensaje(mensaje);

		return ERROR_DESCONEXION_CPU;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("Ocurrió un error al recibir el mensaje de cpu");

		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("Mensaje desconocido desde cpu, Código: %d", mensaje->head);

		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}

