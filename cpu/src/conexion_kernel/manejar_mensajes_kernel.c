#include "manejar_mensajes_kernel.h"

//Funciones Privadas
void desconexion(t_prot_mensaje * mensaje);


// Hilos: 0-N, Crea: 0
int manejar_mensaje_kernel(t_prot_mensaje * mensaje) {
	
	t_pcb * pcb;
	void * pcb_serializada;
	int * valor;
	int codigo_mensaje;
		switch (mensaje->head) {
		case MENSAJE_DISPATCH_K_CPU:;
			valor = malloc(sizeof(int));
			loggear_trace("Me llegó una PCB. La ejecuto.");
			pcb = deserializarPCB(mensaje->payload, valor);
    		ejecutar_ciclo_instruccion(pcb);
			*valor = get_ciclosIO();
			pcb_serializada = serializar_pcb(pcb,*valor);
			codigo_mensaje = enviar_mensaje_protocolo(mensaje->socket,RESPUESTA_DISPATCH_CPU_K,sizeof(t_instruccion) * list_size(pcb->instrucciones) +  sizeof(uint32_t) + sizeof(uint32_t) * 4 + sizeof(float) + sizeof(int),pcb_serializada);
			if (codigo_mensaje < 0) {
				loggear_error("Ocurrio un error al mandar la informacion");
			} else {
				loggear_info("Se mando a kernel la info solicitada");
			}
			//desconexion(mensaje);
			destruir_mensaje(mensaje);
			destructor_PCB(pcb);
			free(pcb_serializada);
			free(valor);

			return 0;
			break;
		case MENSAJE_INTERRUPT_K_CPU:;
			pthread_mutex_lock(&m_interrupt);
			loggear_warning("Llega un interrupt, se debe devolver la pcb a Kernel");
    		set_interrupcion_kernel_true();
			pthread_mutex_unlock(&m_interrupt);
			break;
		case DESCONEXION_KERNEL_K_CPU:
			loggear_trace("Le mando a memoria que se desconecte");
		case DESCONEXION:
			loggear_trace("Le mando a memoria que se desconecte");
			memoria_cerrar_conexiones(true);			
			
			destruir_mensaje(mensaje);

			return ERROR_DESCONEXION_KERNEL;
			break;
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
	loggear_info("Se cierra el socket de kernel!");

	close(mensaje->socket);
}

