
#include "kernel_configuracion_guardada.h"

// GETTERS//
char * get_ip_memoria() {return config_guardada.ip_memoria; }
int get_puerto_memoria() { return config_guardada.puerto_memoria; }
char * get_ip_cpu() {return config_guardada.ip_cpu; }
int get_puerto_cpu() { return config_guardada.puerto_cpu_dispatch; } //ESTE ES EL PUERTO DE DISPATCH
int get_puerto_cpu_interrupt() { return config_guardada.puerto_cpu_interrupt; }
int get_puerto_escucha() { return config_guardada.puerto_escucha; }
char * get_algoritmo_planificacion() { return config_guardada.algoritmo_planificacion; }
int get_estimacion_inicial() { return config_guardada.estimacion_inicial; }
double get_alfa() { return config_guardada.alfa; }
int get_grado_multiprogramacion() { return config_guardada.grado_multiprogramacion; }
int get_tiempo_maximo_bloqueado() { return config_guardada.tiempo_maximo_bloqueado; }

char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }

