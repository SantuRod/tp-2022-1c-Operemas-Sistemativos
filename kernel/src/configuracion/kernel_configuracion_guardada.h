#ifndef CONFIG_GUARDADA_H_
#define CONFIG_GUARDADA_H_

// STRUCTS
#include <commons/collections/list.h>

typedef struct {
	char * ip_memoria;
	int puerto_memoria;
	char * ip_cpu;
	int puerto_cpu_dispatch;
	int puerto_cpu_interrupt;
	int puerto_escucha;
	char * algoritmo_planificacion;
	int estimacion_inicial;
	double alfa;
	int grado_multiprogramacion;
	int tiempo_maximo_bloqueado;

	// Logging
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;
} t_info_config;


// GETTERS
char * get_ip_memoria();
int get_puerto_memoria();
char * get_ip_cpu();
int get_puerto_cpu_dispatch();
int get_puerto_cpu_interrupt();
int get_puerto_escucha();
char * get_algoritmo_planificacion();
int get_estimacion_inicial();
double alfa();
int get_grado_multiprogramacion();
int get_tiempo_maximo_bloqueado();

char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

int get_puerto_escucha();

char * get_ip_memoria();
int get_puerto_memoria();
char * get_ip_cpu();
int get_puerto_cpu();	//PUERTO DE DISPATCH
int get_puerto_cpu_interrupt();

t_info_config config_guardada;

#endif /* CONFIG_GUARDADA_H_ */
