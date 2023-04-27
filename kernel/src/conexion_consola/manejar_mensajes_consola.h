
#ifndef MANEJAR_MENSAJES_CONSOLA_H_
#define MANEJAR_MENSAJES_CONSOLA_H_

#include <commons/collections/list.h>
#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"
#include "../../../shared/estructura_compartida.h"
#include "../conexion_cpu/conexion_cpu.h"
#include "../kernel_main.h"
#include "../kernel_global.h"
#include "../manejo_planificador.h"
#include "../conexion_memoria/manejar_mensajes_memoria.h"



/**
 * @NAME: manejar_mensaje_consola
 * @DESC: Se le envia un mensaje recibido del consola y ejecuta el código acorde.
 * @RETURN:
 *   0 salió bien
 * 	-1 desconexion
 *	-2 error al recibir
 *	-3 mensaje inentendible
 */
int manejar_mensaje_consola(t_prot_mensaje * mensaje);

void* serializarParaMemoria(uint32_t,uint32_t);

#endif /* MANEJAR_MENSAJES_CONSOLA_H_ */
