
#ifndef PROTOCOLO_H_
#define PROTOCOLO_H_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "logger.h"
#include "codigo_error.h"

//HEADER
typedef enum {
	CONEXION, // Un cliente informa a un servidor que se ha conectado. Payload: Algun t_cliente
	FALLO_AL_RECIBIR,// Indica que un mensaje no se recibio correctamente en prot_recibir_mensaje
	DESCONEXION, // Indica que un cliente se ha desconectado (usado para instancias)

	// Consola => Kernel 3
	DESCONEXION_CONSOLA_CO_K,
	MENSAJE_CO_K,

	// Kernel => Consola 5
	DESCONEXION_KERNEL_K_CO,

	// Kernel => Memoria 6
	INICIALIZACION_K_M,
	DESCONEXION_KERNEL_K_M,
	RESERVAR_MARCOS_K_M,
	FINALIZACION_K_M,
	SUSPENSION_K_M,
	
	// Memoria => Kernel 11
	INICIALIZACION_M_K,
	DESCONEXION_MEMORIA_M_K,
	RESERVA_MARCOS_M_K,
	FINALIZACION_M_K,
	SUSPENSION_M_K,

	// Kernel => CPU 16
	DESCONEXION_KERNEL_K_CPU,
	MENSAJE_DISPATCH_K_CPU,
	MENSAJE_INTERRUPT_K_CPU,

	// CPU => Kernel 19
	RESPUESTA_DISPATCH_CPU_K,
	DESCONEXION_CPU_CPU_K,

	// Memoria => CPU 21
	DESCONEXION_MEMORIA_M_CPU,
	RESPUESTA_CONFIGURACION_M_CPU,
	RESPUESTA_TABLA_NIVEL2_M_CPU,
	RESPUESTA_MARCO_M_CPU,
	RESPUESTA_READ_M_CPU,
	RESPUESTA_WRITE_M_CPU,
	
	// CPU => Memoria 27
	DESCONEXION_CPU_CPU_M,
	PETICION_CONFIGURACION_CPU_M,
	PETICION_TABLA_NIVEL2_CPU_M,
	PETICION_MARCO_CPU_M,
	PETICION_READ_CPU_M,
	PETICION_WRITE_CPU_M,
	MENSAJE_CPU_M

} t_header;

typedef struct{
	t_header head;
	size_t tamanio_total;
	void* payload;
	int socket;
}t_prot_mensaje;

/**
 * @NAME: aceptar_conexion_protocolo
 * @DESC: Recibe un socket enlazado a un puerto (se necesita hacer crear_servidor(puerto) y queda esperando a que llegue un cliente.
 * @USO: while (aceptar_conexion_protocolo > 0) { Código para recivir un mensaje }
 */
int aceptar_conexion_protocolo(int socket_servidor);

/**
* @NAME: enviar_mensaje
* @DESC: Recibe SOCKET destino, HEADER a enviar, sizeof del PAYLOAD y el puntero al PAYLOAD
* @PARAMS
* 		socket_destino - Entero al socket
* 		header - Header del mensaje, es un ENUM. Lo lee primero el que recibe
* 		tamanio_payload - El sizeof, no lo podemos calcular acá dentro porque es un puntero de tipo void*
* 		payload - El payload como tal
*
* @EJEMPLO: Enviar un int, con el header NUMERO
*		 int a = 8
*		 prot_enviar_mensaje( destino, NUMERO, sizeof(int), &a);
*/
int enviar_mensaje_protocolo(int socket_destino, t_header header , size_t tamanio_payload, void* payload);

/**
* @NAME: recibir_mensaje
* @DESC: Retorna un mensaje recibido en el socket que se le pasó por parametro.
* @RETURN:
* 	Tamaño: { HEADER + PAYLOAD }
* 	Dentro del header se encuentra el tamaño del payload.
*
* 	Al recibirlo se tiene que castear.
* 	t_prot_mensaje* mensaje 	= recibir_mensaje_protocolo(numero_de_socket);
* 	t_header header			 	= mensaje->head;
* 	tu_tipo_de_struct payload 	= *(tu_tipo_de_struct*) mensaje->payload;
*/
t_prot_mensaje* recibir_mensaje_protocolo(int socket_origen);

/**
 * @NAME: obtener_string_mensaje
 * @DESC: Cuando el payload es un string y no debe ser casteado en ninguna estructura.
 * @RETURN: Retorna el string, se debe hacer un free(...) luego de usarlo
 */
char* obtener_string_mensaje(t_prot_mensaje* mensaje);

/**
 * @NAME: destruir_mensaje
 * @DESC: Limpia de la memoria al mensaje
 */
void destruir_mensaje(t_prot_mensaje* mensaje);

#endif /* PROTOCOLO_H_ */
