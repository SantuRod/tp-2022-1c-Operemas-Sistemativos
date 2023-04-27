#include "cpu_configuracion.h"


// Declaracion de funciones privadas
int set_variable_str(t_config * config, char * param_leer, char ** param); // Se usa para setear un string leyendo del archivo config

int set_variable_int(t_config * config, char * param_leer, int * param); // Se usa para setear un int leyendo del archivo config

int cargar_archivo(); // carga en config guardada la info que se adquire del .config

int set_variable_list_config(t_config * config, char * param_leer, t_list** param);

void liberar_strings(void* elString);





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

int pedir_info_memoria(){
	int socket_memoria = iniciar_conexion_memoria();

	int codigo_mensaje = enviar_mensaje_protocolo(socket_memoria, PETICION_CONFIGURACION_CPU_M, 0, NULL);

	if (codigo_mensaje < 0) {
		loggear_error("Ocurrio un error enviar la peticion");
	}else{
		loggear_info("Se pidio a Memoria el tamaño de pagina y la entradas por tablas");
	}

	t_prot_mensaje * mensaje = recibir_mensaje_protocolo(socket_memoria);

	uint32_t* tam_pag = malloc(sizeof(uint32_t));
	memcpy(tam_pag, mensaje->payload, sizeof(uint32_t));
	loggear_info("Recibimos que el tamaño de pagina es de %zu bytes", *tam_pag);

	uint32_t* ent_x_tabla = malloc(sizeof(uint32_t));
	memcpy(ent_x_tabla, mensaje->payload + sizeof(uint32_t), sizeof(uint32_t));
	loggear_info("Recibimos hay %zu entradas por tabla", *ent_x_tabla);

    tamano_pagina = *tam_pag;
	cant_entradas_por_tabla = *ent_x_tabla;

	close(socket_memoria);
	free(tam_pag);
	free(ent_x_tabla);
	destruir_mensaje(mensaje);

	return codigo_mensaje;
}

// Publica
bool son_validos_los_parametros(int argc, char ** argv) {
	return argc >= 2 && string_ends_with(argv[1], ".config");
}

// Publica
void destroy_configuracion() {
	free(config_guardada.reemplazo_tlb);
	free(config_guardada.ip_memoria);
	free(config_guardada.log_route);
	free(config_guardada.log_app_name);
}

int cargar_archivo(char * path) {
	t_config * config = config_create(path);
	int error = 0;

	error += set_variable_int(config, "ENTRADAS_TLB",				&config_guardada.entradas_tlb);
	error += set_variable_str(config, "REEMPLAZO_TLB", 				&config_guardada.reemplazo_tlb);
	error += set_variable_int(config, "RETARDO_NOOP",				&config_guardada.retardo_noop);
	error += set_variable_str(config, "IP_MEMORIA", 				&config_guardada.ip_memoria);
	error += set_variable_int(config, "PUERTO_MEMORIA", 			&config_guardada.puerto_memoria);
	error += set_variable_int(config, "PUERTO_ESCUCHA_DISPATCH",	&config_guardada.puerto_escucha_dispatch);
	error += set_variable_int(config, "PUERTO_ESCUCHA_INTERRUPT",	&config_guardada.puerto_escucha_interrupt);

	// Para loggear
	error += set_variable_str(config, "LOG_ROUTE", 					&config_guardada.log_route);
	error += set_variable_str(config, "LOG_APP_NAME", 				&config_guardada.log_app_name);
	error += set_variable_int(config, "LOG_IN_CONSOLE", 			&config_guardada.log_in_console);
	error += set_variable_int(config, "LOG_LEVEL_INFO", 			&config_guardada.log_level_info);


	if (error != 0) {
		return -2;
	}

	config_destroy(config);

	return 0;
}

void liberar_strings(void* elString){
	free(elString);
}

int set_variable_int(t_config * config, char * param_leer, int * param) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	*param = config_get_int_value(config, param_leer);

	return 0;
}

int set_variable_dbl(t_config * config, char * param_leer, double * param){
	if(!config_has_property(config, param_leer)){
		return CONFIG_ERROR_EN_ARCHIVO;
	}
	*param = config_get_double_value(config, param_leer);

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

int set_variable_list_config(t_config * config, char * param_leer, t_list** param) {
	if (!config_has_property(config, param_leer)) {
		return CONFIG_ERROR_EN_ARCHIVO;
	}

	char** variable = config_get_array_value(config, param_leer);

	int i = 0;

	if(variable[i] == NULL){
		return 0;
	}else{

	list_replace(*param, 0, variable[i]);
	i++;
		while(variable[i] != NULL){
			list_add(*param, variable[i]);
			i++;
		}

	}

	free(variable);

	return 0;
}

