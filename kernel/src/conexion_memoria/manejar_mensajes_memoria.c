#include "manejar_mensajes_memoria.h"

//Funciones Privadas
int manejar_mensajes_memoria(t_prot_mensaje * mensaje);

int recibir_mensaje_memoria(int socket_memoria) {
	t_prot_mensaje * mensaje_memoria = recibir_mensaje_protocolo(socket_memoria);

	return manejar_mensajes_memoria(mensaje_memoria);
}

int manejar_mensajes_memoria(t_prot_mensaje * mensaje) {
	int resultado;
	switch (mensaje->head) {
	case SUSPENSION_M_K:;
		int* ressuspencion = malloc(sizeof(int));
		memcpy(ressuspencion,mensaje->payload,sizeof(uint32_t));
		resultado = *ressuspencion;
		free(ressuspencion);
		destruir_mensaje(mensaje);
		return resultado;
		break;
	case FINALIZACION_M_K:;
		int* resfinalizacion = malloc(sizeof(int));
		memcpy(resfinalizacion,mensaje->payload,sizeof(uint32_t));
		resultado = *resfinalizacion;
		free(resfinalizacion);
		destruir_mensaje(mensaje);
		return resultado;
		break;
	case INICIALIZACION_M_K:;
		int* loquedevuelvo = malloc(sizeof(int));
		memcpy(loquedevuelvo,mensaje->payload,sizeof(uint32_t));
		resultado = *loquedevuelvo;
		free(loquedevuelvo);
		destruir_mensaje(mensaje);
		return resultado;
	break;
	case RESERVA_MARCOS_M_K:
		loggear_info("Resultado de la op = %zu",*(uint32_t*)mensaje->payload);
		resultado = *(uint32_t*)mensaje->payload;
		destruir_mensaje(mensaje);
		return resultado;
	break;
	case DESCONEXION_MEMORIA_M_K:
	case DESCONEXION:
		loggear_warning("Se cerró la conexión con memoria");

		destruir_mensaje(mensaje);

		return ERROR_DESCONEXION_MEMORIA;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("Ocurrió un error al recibir el mensaje de memoria");

		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("Mensaje desconocido desde memoria, Código: %d", mensaje->head);

		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}