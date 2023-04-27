#include "manejar_mensajes_cpu_y_kernel.h"

//Funciones Privadas
void desconexion(t_prot_mensaje * mensaje);


// Hilos: 0-N, Crea: 0
int manejar_mensaje_cpu_y_kernel(t_prot_mensaje * mensaje) {
	
	uint32_t* indice;
	uint32_t* direccion;
	uint32_t* direccion_n2;
	uint32_t* marco;
	uint32_t* info;
	uint32_t* resultado;

	int codigo_mensaje;
		switch (mensaje->head) {
		case SUSPENSION_K_M:;
			resultado = malloc(sizeof(uint32_t));
			*resultado = sacar_de_memoria(*(int*)mensaje->payload, false);
			loggear_trace("Resultado = %zu",*resultado);

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, SUSPENSION_M_K, sizeof(uint32_t), (void*) resultado);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion SUSPENSION_M_K,");
			} else {
				loggear_info("Se mando al kernel la info solicitada");
			}
			destruir_mensaje(mensaje);
			free(resultado);
			break;
		case FINALIZACION_K_M:;
			resultado = malloc(sizeof(uint32_t));
			*resultado = cerrar_proceso(*(int*)mensaje->payload);
			loggear_info("Mato al proceso");

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, FINALIZACION_M_K, sizeof(uint32_t), (void*) resultado);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion FINALIZACION_M_K,");
			} else {
				loggear_info("Se mando al kernel la info solicitada");
			}
			destruir_mensaje(mensaje);
			free(resultado);
			break;
		case INICIALIZACION_K_M:;
			uint32_t* tamProceso = malloc(sizeof(uint32_t));
			uint32_t* idProceso = malloc(sizeof(uint32_t));
			memcpy(tamProceso,mensaje->payload,sizeof(uint32_t));
			memcpy(idProceso,mensaje->payload+sizeof(uint32_t),sizeof(uint32_t));
			
			uint32_t* mensajito = malloc(sizeof(uint32_t));
			*mensajito = crear_estructura_proceso_nuevo(*idProceso,*tamProceso);

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, INICIALIZACION_M_K, sizeof(uint32_t), (void*) mensajito);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion INICIALIZACION_M_K,");
			} else {
				loggear_info("Se mando al kernel la info solicitada");
			}
			destruir_mensaje(mensaje);
			free(idProceso);
			free(tamProceso);
			free(mensajito);
			break;
		case RESERVAR_MARCOS_K_M:;
			indice = malloc(sizeof(uint32_t));
			resultado = malloc(sizeof(uint32_t));
			memcpy(indice,mensaje->payload,sizeof(uint32_t));
			*resultado = meter_proceso_en_memoria(*indice);
			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, RESERVA_MARCOS_M_K, sizeof(uint32_t), (void*) resultado);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion INICIALIZACION_M_K,");
			} else {
				loggear_info("Se mando al kernel la info solicitada");
			}
			destruir_mensaje(mensaje);
			free(resultado);
			free(indice);
			break;
		case PETICION_CONFIGURACION_CPU_M:;
			void* info_serializada = malloc(sizeof(uint32_t)*2);
			uint32_t* tam_pag = malloc(sizeof(uint32_t)); 
			uint32_t* ent_x_tabla = malloc(sizeof(uint32_t)); 
			*tam_pag = get_tam_pagina();
			*ent_x_tabla = get_paginas_por_tabla();

			memcpy(info_serializada, tam_pag, sizeof(uint32_t));
			memcpy(info_serializada + sizeof(uint32_t), ent_x_tabla, sizeof(uint32_t));

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, RESPUESTA_CONFIGURACION_M_CPU, sizeof(uint32_t)*2, info_serializada);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion");
			} else {
				loggear_info("Se mando a la cpu la info solicitada");
			}

			destruir_mensaje(mensaje);
			free(tam_pag);
			free(ent_x_tabla);
			free(info_serializada);
			
			break;
		case PETICION_TABLA_NIVEL2_CPU_M:;

			indice = malloc(sizeof(uint32_t));
			memcpy(indice, mensaje->payload, sizeof(uint32_t));
			loggear_info("Recibimos pedido n2 del proceso con indice %zu", *indice);

			direccion = malloc(sizeof(uint32_t));
			memcpy(direccion, mensaje->payload + sizeof(uint32_t), sizeof(uint32_t));
			loggear_info("Para leer en la direccion %zu", *direccion);

		

			direccion_n2 = malloc(sizeof(uint32_t));
			*direccion_n2 = get_pagina_n2(*indice, *direccion);
			loggear_info("La pagina esta en la direccion %zu", *direccion_n2);

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, RESPUESTA_TABLA_NIVEL2_M_CPU, sizeof(uint32_t), direccion_n2);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion");
			} else {
				loggear_info("Se mando a la cpu la info solicitada");
			}

			destruir_mensaje(mensaje);
			free(indice);
			free(direccion);
			free(direccion_n2);

			

			break;
		case PETICION_MARCO_CPU_M:;
			indice = malloc(sizeof(uint32_t));
			memcpy(indice, mensaje->payload, sizeof(uint32_t));
			loggear_info("Recibimos pedido marco del proceso con indice %zu", *indice);

			direccion_n2 = malloc(sizeof(uint32_t));
			memcpy(direccion_n2, mensaje->payload + sizeof(uint32_t), sizeof(uint32_t));
			loggear_info("Para leer en la direccion %zu", *direccion_n2);

			marco = malloc(sizeof(uint32_t));
			*marco = get_marco_de_pagina(*indice, *direccion_n2);
			loggear_info("La pagina esta en el marco %zu", *marco);

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, RESPUESTA_MARCO_M_CPU, sizeof(uint32_t), marco);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion");
			} else {
				loggear_info("Se mando a la cpu la info solicitada");
			}

			destruir_mensaje(mensaje);
			free(indice);
			free(direccion_n2);
			free(marco);

			

			break;
		case PETICION_READ_CPU_M:;

			direccion = malloc(sizeof(uint32_t));
			memcpy(direccion, mensaje->payload, sizeof(uint32_t));
			loggear_info("Se nos pidio leer en la direccion %zu", *direccion);

			info = malloc(sizeof(uint32_t));
			*info = leer_memoria(*direccion);
			loggear_info("Se leyo el valor %zu", *info);

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, RESPUESTA_READ_M_CPU, sizeof(uint32_t), info);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion");
			} else {
				loggear_info("Se mando a la cpu la info solicitada");
			}

			destruir_mensaje(mensaje);
			free(direccion);
			free(info);

			

			break;
		case PETICION_WRITE_CPU_M:;
			direccion = malloc(sizeof(uint32_t));
			memcpy(direccion, mensaje->payload, sizeof(uint32_t));
			loggear_info("Se nos pidio escribir en la direccion %zu", *direccion);
			
			info = malloc(sizeof(uint32_t));
			memcpy(info, mensaje->payload + sizeof(uint32_t), sizeof(uint32_t));
			loggear_info("Escribimos el valor %zu", *info);


			resultado = malloc(sizeof(uint32_t));
			*resultado = escribir_memoria(*direccion, *info);
			loggear_info("Resultado de la escritura: %zu", *resultado);

			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket, RESPUESTA_MARCO_M_CPU, sizeof(uint32_t), resultado);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion");
			} else {
				loggear_info("Se mando a la cpu la info solicitada");
			}

			destruir_mensaje(mensaje);
			free(info);
			free(direccion);
			free(resultado);

			

			break;
		case MENSAJE_CPU_M:;
			char* texto = mensaje->payload;
			loggear_trace("me llego %s", texto);
			
			destruir_mensaje(mensaje);
			
			break;
		case DESCONEXION_KERNEL_K_M:
			desconexion(mensaje);
			
			
			destruir_mensaje(mensaje);

			return ERROR_DESCONEXION_KERNEL;
		case DESCONEXION_CPU_CPU_M:
		case DESCONEXION:
			destruir_mensaje(mensaje); 

			return ERROR_DESCONEXION_CPU;
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

	return 0;
}

void desconexion(t_prot_mensaje * mensaje) {
	loggear_info("Se desconecto uno de los sockets!");

	close(mensaje->socket);
}

