
#ifndef MANEJAR_MENSAJES_CPU_Y_KERNEL_H_
#define MANEJAR_MENSAJES_CPU_Y_KERNEL_H_


#include <arpa/inet.h>
#include <unistd.h>

#include "../paginacion/paginacion.h"
#include "../manejo_memoria/manejo_memoria.h"
#include "../configuracion/memoria_config_guardada.h"
#include "../../../shared/logger.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/codigo_error.h"
#include "../../../shared/thread_safe_value.h"
#include "../../../shared/estructura_compartida.h"

int manejar_mensaje_cpu_y_kernel(t_prot_mensaje * mensaje);

#endif /* MANEJAR_MENSAJES_CPU_Y_KERNEL_H_ */
