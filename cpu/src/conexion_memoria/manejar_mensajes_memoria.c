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
	case INICIALIZACION_M_K:
		loggear_info("Recibimos tabla de paginas de kernel %zu", *(uint32_t*) mensaje->payload);

		int direccion_tabla_paginas = *(uint32_t*) mensaje->payload;
		destruir_mensaje(mensaje);
		return direccion_tabla_paginas;
	
	case RESPUESTA_TABLA_NIVEL2_M_CPU:
		loggear_info("Recibimos que la tabla de nivel 2 esta en %zu", *(uint32_t*) mensaje->payload);

		int direccion_tabla_nivel2 = *(uint32_t*) mensaje->payload;

		destruir_mensaje(mensaje);
		return direccion_tabla_nivel2;
	case RESPUESTA_MARCO_M_CPU:
		loggear_info("Recibimos que la pagina esta en el marco %zu", *(uint32_t*) mensaje->payload);

		int marco = *(uint32_t*) mensaje->payload;

		destruir_mensaje(mensaje);
		return marco;
	case RESPUESTA_READ_M_CPU:
		loggear_info("Recibimos que la en la direccion esta la info: %zu", *(uint32_t*) mensaje->payload);

		resultado = *(uint32_t*) mensaje->payload;

		destruir_mensaje(mensaje);
		return resultado;
	case RESPUESTA_WRITE_M_CPU:
		loggear_info("Recibimos resultado de la escritura: %zu", *(uint32_t*) mensaje->payload);

		resultado = *(uint32_t*) mensaje->payload;

		destruir_mensaje(mensaje);
		return resultado;

	case DESCONEXION_MEMORIA_M_CPU:
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

int pedir_direccion_tabla_nivel2(uint32_t indice, uint32_t direccion){

	void* info_serializada = malloc(sizeof(uint32_t)*2);

	uint32_t* i = malloc(sizeof(uint32_t)); 
	uint32_t* d = malloc(sizeof(uint32_t)); 
	*i = indice;
	*d = direccion;

	memcpy(info_serializada, i, sizeof(uint32_t));
	memcpy(info_serializada + sizeof(uint32_t), d, sizeof(uint32_t));

	int socket_memoria = iniciar_conexion_memoria();

	int codigo_mensaje = enviar_mensaje_protocolo(socket_memoria, PETICION_TABLA_NIVEL2_CPU_M, sizeof(uint32_t)*2, info_serializada);
	if (codigo_mensaje < 0) {
		loggear_error("Ocurrio un error enviar la peticion");
	}else{
		loggear_info("Se pidio a Memoria la direccion de la tabla de segundo nivel en la direccion %zu", direccion);
	}

	int direccion_tabla_n2 = recibir_mensaje_memoria(socket_memoria);
	loggear_info("La tabla de segundo nivel empieza en %zu", direccion_tabla_n2);

	free(i);
	free(d);
	free(info_serializada);
	close(socket_memoria);
	return direccion_tabla_n2;
}

int pedir_marco(uint32_t indice, uint32_t direccion){

	void* info_serializada = malloc(sizeof(uint32_t)*2);

	uint32_t* i = malloc(sizeof(uint32_t)); 
	uint32_t* d = malloc(sizeof(uint32_t)); 
	*i = indice;
	*d = direccion;

	memcpy(info_serializada, i, sizeof(uint32_t));
	memcpy(info_serializada + sizeof(uint32_t), d, sizeof(uint32_t));

	int socket_memoria = iniciar_conexion_memoria();

	int codigo_mensaje = enviar_mensaje_protocolo(socket_memoria, PETICION_MARCO_CPU_M, sizeof(uint32_t)*2, info_serializada);
	if (codigo_mensaje < 0) {
		loggear_error("Ocurrio un error enviar la peticion");
	}else{
		loggear_info("Se pidio a Memoria la direccion de la tabla de segundo nivel en la direccion %zu", direccion);
	}

	int marco = recibir_mensaje_memoria(socket_memoria);
	loggear_info("La tabla de segundo nivel esta en el marco %zu", marco); //hay que ver que pasa cuando devuelve -1

	free(i);
	free(d);
	free (info_serializada);
	close(socket_memoria);
	return marco;
}

int pedir_read(uint32_t direccion){

	uint32_t* d = malloc(sizeof(uint32_t));
	*d = direccion;

	int socket_memoria = iniciar_conexion_memoria();

	int codigo_mensaje = enviar_mensaje_protocolo(socket_memoria, PETICION_READ_CPU_M, sizeof(uint32_t)*2, d);
	if (codigo_mensaje < 0) {
		loggear_error("Ocurrio un error enviar la peticion");
	}else{
		loggear_warning("Se pidio a Memoria leer la direccion física %zu", *d);
	}

	uint32_t resultado = (uint32_t) recibir_mensaje_memoria(socket_memoria);
	loggear_trace("Se leyo el valor %zu", resultado);

	free(d);
	close(socket_memoria);
	return resultado;
}

int pedir_write(uint32_t direccion, uint32_t valor){

	void* info_serializada = malloc(sizeof(uint32_t)*2);

	uint32_t* d = malloc(sizeof(uint32_t)); 
	uint32_t* v = malloc(sizeof(uint32_t)); 
	*d = direccion;
	*v = valor;

	memcpy(info_serializada, d, sizeof(uint32_t));
	memcpy(info_serializada + sizeof(uint32_t), v, sizeof(uint32_t));

	int socket_memoria = iniciar_conexion_memoria();

	int codigo_mensaje = enviar_mensaje_protocolo(socket_memoria, PETICION_WRITE_CPU_M, sizeof(uint32_t)*2, info_serializada);
	if (codigo_mensaje < 0) {
		loggear_error("Ocurrio un error enviar la peticion");
	}else{
		loggear_warning("Se pidio a Memoria escribir en la dirección física %zu", direccion);
	}

	uint32_t resultado = (uint32_t) recibir_mensaje_memoria(socket_memoria);
	loggear_info("Resultado de la escritura %zu", resultado);

	free(d);
	free(v);
	free(info_serializada);
	close(socket_memoria);
	return resultado;
}
