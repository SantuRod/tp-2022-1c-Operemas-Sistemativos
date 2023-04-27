
#include "memoria_config_guardada.h"

int get_puerto_escucha() { return config_guardada.puerto_escucha; }
int get_tam_memoria() { return config_guardada.tam_memoria; }
int get_tam_pagina() { return config_guardada.tam_pagina; }
int get_paginas_por_tabla() { return config_guardada.paginas_por_tabla; }
int get_retardo_memoria() { return config_guardada.retardo_memoria; }
char * get_algoritmo_reemplazo() { return config_guardada.algoritmo_reemplazo; }
int get_marcos_por_proceso() { return config_guardada.marcos_por_proceso; }
int get_retardo_swap() { return config_guardada.retardo_swap; }
char * get_path_swap() { return config_guardada.path_swap; }

char * get_log_route() { return config_guardada.log_route; }
char * get_log_app_name() { return config_guardada.log_app_name; }
int get_log_in_console() { return config_guardada.log_in_console; }
int get_log_level_info() { return config_guardada.log_level_info; }


