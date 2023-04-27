
#ifndef MEMORIA_MAIN_H_
#define MEMORIA_MAIN_H_

#include "configuracion/memoria_configuracion.h"
#include "../../shared/logger.h"
#include "../../shared/conexion.h"
#include "../../shared/protocolo.h"

#include <commons/collections/list.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include <readline/readline.h>

#include "./paginacion/paginacion.h"
#include "./manejo_memoria/manejo_memoria.h"
#include "./inicializacion/inicializacion.h"
#include "./conexion_cpu_y_kernel/conexion_cpu_y_kernel.h"
#include "./conexion_cpu_y_kernel/coordinar_hilos.h"

#endif /* MEMORIA_MAIN_H_ */
