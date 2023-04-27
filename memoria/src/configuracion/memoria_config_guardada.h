#ifndef CONFIGURACION_MEMORIA_CONFIG_GUARDADA_H_
#define CONFIGURACION_MEMORIA_CONFIG_GUARDADA_H_



// GETTERS
int get_puerto_escucha();
int get_tam_memoria();
int get_tam_pagina();
int get_paginas_por_tabla();
int get_retardo_memoria();
char * get_algoritmo_reemplazo();
int get_marcos_por_proceso();
int get_retardo_swap();
char * get_path_swap();

char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

typedef struct {

	int puerto_escucha;
	int tam_memoria;
	int tam_pagina;
	int paginas_por_tabla;
	int retardo_memoria;
	char * algoritmo_reemplazo;
	int marcos_por_proceso;
	int retardo_swap;
	char * path_swap;

	// Logging
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;

} t_info_config;


t_info_config config_guardada;

#endif /* CONFIGURACION_MEMORIA_CONFIG_GUARDADA_H_ */
