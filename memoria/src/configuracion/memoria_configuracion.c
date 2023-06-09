#include "memoria_configuracion.h"

// Declaracion de funciones privadas
int set_variable_str(t_config * config, char * param_leer, char ** param); // Se usa para setear un string leyendo del archivo config

int set_variable_int(t_config * config, char * param_leer, int * param); // Se usa para setear un int leyendo del archivo config

int set_variable_dbl(t_config * config, char * param_leer, double * param); // Se usa para setear un dobule leyendo del archivo config

int cargar_archivo(); // carga en config guardada la info que se adquire del .config

// Publica
int iniciar_configuracion(int argc, char ** argv) {

	const bool es_config_valida = son_validos_los_parametros(argc, argv);

	const char * path_config = argv[1];

	if (!es_config_valida) {
		return CONFIG_ARGUMENTOS_INVALIDOS;
	}

	int error = cargar_archivo(path_config);
	if (error != 0) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	return 0;
}

// Publica
bool son_validos_los_parametros(int argc, char ** argv) {
	return argc >= 2 && string_ends_with(argv[1], ".config");
}

// Publica
void destroy_configuracion() {
	free(config_guardada.algoritmo_reemplazo);
	free(config_guardada.path_swap);
	free(config_guardada.log_route);
	free(config_guardada.log_app_name);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;


	error += set_variable_int(config, "PUERTO_ESCUCHA", 	&config_guardada.puerto_escucha);
	error += set_variable_int(config, "TAM_MEMORIA", 		&config_guardada.tam_memoria);
	error += set_variable_int(config, "TAM_PAGINA", 		&config_guardada.tam_pagina);
	error += set_variable_int(config, "PAGINAS_POR_TABLA", 	&config_guardada.paginas_por_tabla);
	error += set_variable_int(config, "RETARDO_MEMORIA", 	&config_guardada.retardo_memoria);
	error += set_variable_str(config, "ALGORITMO_REEMPLAZO",&config_guardada.algoritmo_reemplazo);
	error += set_variable_int(config, "MARCOS_POR_PROCESO", &config_guardada.marcos_por_proceso);
	error += set_variable_int(config, "RETARDO_SWAP", 		&config_guardada.retardo_swap);
	error += set_variable_str(config, "PATH_SWAP", 			&config_guardada.path_swap);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 			&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 		&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 	&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 	&config_guardada.log_level_info);


	if (error != 0) {
		return -2;
	}

	config_destroy(config);

	return 0;
}

int set_variable_int(t_config * config, char * param_leer, int * param) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	*param = config_get_int_value(config, param_leer);

	return 0;
}

int set_variable_str(t_config * config, char * param_leer, char ** param) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	char * variable_auxiliar = config_get_string_value(config, param_leer);

	*param = malloc(sizeof(char) * (string_length(variable_auxiliar) + 1));

	strcpy( *param, variable_auxiliar );

	return 0;
}
