
#include "consola_configuracion_guardada.h"

// GETTERS//
char * get_ip_kernel() {return config_guardada.ip_kernel; }
int get_puerto_kernel() { return config_guardada.puerto_kernel; }


char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }

