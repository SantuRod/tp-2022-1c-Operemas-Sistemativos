#include "manejar_mensajes_kernel.h"


void InstruccionIntoStream (void* stream, t_instruccion* instruccion);



int recibir_mensaje_kernel(int socket_kernel) {
	t_prot_mensaje * mensaje_kernel = recibir_mensaje_protocolo(socket_kernel);

	return manejar_mensajes_kernel(mensaje_kernel);
}

int manejar_mensajes_kernel(t_prot_mensaje * mensaje) {
	switch (mensaje->head) {
	case DESCONEXION_KERNEL_K_CO:
	case DESCONEXION:
		loggear_warning("Se cerró la conexión y el socket con kernel");
		close(mensaje->socket);
		destruir_mensaje(mensaje);

		return ERROR_DESCONEXION_KERNEL;
	break;
	case FALLO_AL_RECIBIR:
		loggear_error("Ocurrió un error al recibir el mensaje de kernel");
		close(mensaje->socket);
		destruir_mensaje(mensaje);

		return ERROR_AL_RECIBIR_MENSAJE;
	break;
	default:
		loggear_error("Mensaje desconocido desde kernel, Código: %d", mensaje->head);
		close(mensaje->socket);
		destruir_mensaje(mensaje);

		return ERROR_MENSAJE_DESCONOCIDO;
	break;
	}
}

//Mensajes Consola => Kernel
void* serializarInstruccionesConsola (t_list* instrucciones, uint32_t tam_consola, size_t* tam_final) {
	void* paquete = malloc(sizeof(t_instruccion) * list_size(instrucciones) + 2 * sizeof(uint32_t));
	size_t tamanio_buffer = sizeof(t_instruccion) * list_size(instrucciones) + 2 * sizeof(uint32_t);
	size_t offset = 0;

	uint32_t cantInstrucciones;
	cantInstrucciones =  list_size(instrucciones);
	memcpy(paquete + offset, &cantInstrucciones, sizeof(uint32_t));
	offset+=sizeof(uint32_t);

	for(int i=0;i<list_size(instrucciones);i++) {
		t_instruccion* instruccion = list_get(instrucciones,i);
		memcpy(paquete + offset, (uint32_t*) &instruccion->id_instruccion,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(paquete + offset, (uint32_t*) &instruccion->primerOperando,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(paquete + offset, (uint32_t*) &instruccion->segundoOperando,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
	}

	uint32_t* tam_consola_puntero = &tam_consola;
	memcpy(paquete + offset, tam_consola_puntero, sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	
	if (tam_final != NULL) {
		*tam_final = tamanio_buffer;
	}
	return paquete;	
}
