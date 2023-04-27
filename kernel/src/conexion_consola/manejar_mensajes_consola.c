#include "manejar_mensajes_consola.h"

//Funciones Privadas
void desconexion(t_prot_mensaje* mensaje);
int contadorPID = 0;

// Hilos: 0-N, Crea: 0
int manejar_mensaje_consola(t_prot_mensaje * mensaje) {
	bool cerrarConexion = false;
		switch (mensaje->head) {
		case MENSAJE_CO_K: ;
		
			size_t offset = 0;
			t_pcb* pcbInicial = malloc(sizeof(t_pcb));
			pcbInicial->id = contadorPID++;

			uint32_t* cantInstrucciones = malloc(sizeof(uint32_t));
			memcpy(cantInstrucciones,mensaje->payload,sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			
			pcbInicial->instrucciones = list_create();

			for(int i=0;i<*cantInstrucciones;i++) {
				t_instruccion* instruccion = malloc(sizeof(t_instruccion));
				memcpy(&instruccion->id_instruccion, mensaje->payload+offset,sizeof(uint32_t));
				offset+=sizeof(uint32_t);
				memcpy(&instruccion->primerOperando, mensaje->payload+offset,sizeof(uint32_t));
				offset+=sizeof(uint32_t);
				memcpy(&instruccion->segundoOperando, mensaje->payload+offset,sizeof(uint32_t));
				offset+=sizeof(uint32_t);
				list_add(pcbInicial->instrucciones, instruccion);
			}
			
			memcpy(&pcbInicial->tam_proceso,mensaje->payload+offset,sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			pcbInicial->program_counter = 0;


			int socket_memoria = iniciar_conexion_memoria();
			void* paquete = serializarParaMemoria(pcbInicial->tam_proceso,pcbInicial->id);
			enviar_mensaje_protocolo(socket_memoria,INICIALIZACION_K_M,sizeof(uint32_t)*2,paquete);

			pcbInicial->tabla_paginas = recibir_mensaje_memoria(socket_memoria);
			close(socket_memoria);
			loggear_warning("Tabla de páginas = %d", pcbInicial->tabla_paginas);
			pcbInicial->estimacion_rafaga = config_guardada.estimacion_inicial;

			agregar_pcb_a_new(pcbInicial,mensaje->socket);
			free(paquete);
			free(cantInstrucciones);
			break;
		case DESCONEXION_CONSOLA_CO_K:
			loggear_trace("La consola se desconecta");
			continuar_escuchando = false;
			cerrarConexion = true;
			destruir_mensaje(mensaje);

			return ERROR_DESCONEXION_CONSOLA;
		case FALLO_AL_RECIBIR:
			loggear_error("Error al recibir mensaje");

			desconexion(mensaje);
			destruir_mensaje(mensaje);

			return ERROR_AL_RECIBIR_MENSAJE;
		default:
			desconexion(mensaje);

			loggear_error("Ocurrio un error al recibir el mensaje");

			destruir_mensaje(mensaje);

			return ERROR_MENSAJE_DESCONOCIDO;
		}

		if(cerrarConexion) desconexion(mensaje);
		destruir_mensaje(mensaje);
	return 0;
}



void desconexion(t_prot_mensaje * mensaje) {
	if(close(mensaje->socket))
	loggear_info("Se desconecto socket");
}

void* serializarParaMemoria(uint32_t tam_proceso,uint32_t id) {
	void* paquete = malloc(sizeof(uint32_t)*2);
	memcpy(paquete,&tam_proceso,sizeof(uint32_t));
	memcpy(paquete+sizeof(uint32_t),&id,sizeof(uint32_t));
	return paquete;
}