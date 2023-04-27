#ifndef CONFIGURACION_KERNEL_CONFIG_GUARDADA_H_
#define CONFIGURACION_KERNEL_CONFIG_GUARDADA_H_

// STRUCTS
#include <commons/collections/list.h>

typedef struct {
	char * ip_kernel;
	int puerto_kernel;


	// Loggin
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;



} t_info_config;


// GETTERS
char * get_ip_kernel();
int get_puerto_kernel();

char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();


t_info_config config_guardada;

#endif /* CONFIGURACION_KERNEL_CONFIG_GUARDADA_H_ */
