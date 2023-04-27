#ifndef MANEJAR_MENSAJES_KERNEL_H_
#define MANEJAR_MENSAJES_KERNEL_H_

#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"
#include "../../../shared/estructura_compartida.h"

#include "../ciclo_instruccion/ciclo_instruccion.h"
#include "../conexion_memoria/conexion_memoria.h"


int manejar_mensaje_kernel(t_prot_mensaje * mensaje);

#endif /* MANEJAR_MENSAJES_KERNEL_H_ */
