#include "cpu_configuracion_guardada.h"

// GETTERS//
int get_entradas_tlb() {return config_guardada.entradas_tlb; }
char * get_reemplazo_tlb() { return config_guardada.reemplazo_tlb; }
int get_retardo_noop() {return config_guardada.retardo_noop; }
char * get_ip_memoria() {return config_guardada.ip_memoria; }
int get_puerto_memoria() { return config_guardada.puerto_memoria; }
int get_puerto_escucha_dispatch() { return config_guardada.puerto_escucha_dispatch; }
int get_puerto_escucha_interrupt() { return config_guardada.puerto_escucha_interrupt; }

char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }

