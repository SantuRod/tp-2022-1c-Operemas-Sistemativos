
#ifndef CPU_CONFIGURACION_H_
#define CPU_CONFIGURACION_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <commons/config.h>
#include <commons/string.h>

#include "cpu_configuracion_guardada.h"


#include "../conexion_memoria/conexion_memoria.h"

#include "../../../shared/codigo_error.h"
#include "../../../shared/protocolo.h"
#include "../../../shared/logger.h"



/**
 * @NAME: son_validos_los_parametros
 * @DESC: Retorna true si hay 2 argumentos y el 2do termina en ".config"
 *
 * Ejemplo:
 * son_validos_los_parametros(2, ["...", "a.config"]) = true
 * son_validos_los_parametros(2, ["...", "a.no_config"]) = false
 * son_validos_los_parametros(1, ["..."]) = false
 */
bool son_validos_los_parametros(int argc, char ** argv);

/**
 * @NAME: iniciar_configuracion
 * @DESC: Lee el archivo de configuracion que pasan por parametro e inicializa las variables
 */
int iniciar_configuracion(int argc, char ** argv);

/**
 * @NAME: destroy_configuracion
 * @DESC: Libera la memoria de todos los punteros de la variable "global" config_guardada
 */

void destroy_configuracion();

#endif /* CPU_CONFIGURACION_H_ */
