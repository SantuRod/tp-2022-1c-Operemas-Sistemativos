#ifndef CONFIGURACION_CPU_CONFIG_GUARDADA_H_
#define CONFIGURACION_CPU_CONFIG_GUARDADA_H_

// STRUCTS
#include <arpa/inet.h>

#include <commons/collections/list.h>

uint32_t tamano_pagina;
uint32_t cant_entradas_por_tabla;

typedef struct {
	int entradas_tlb;
	char * reemplazo_tlb;
	int retardo_noop;
	char * ip_memoria;
	int puerto_memoria;
	int puerto_escucha_dispatch;
	int puerto_escucha_interrupt;

	// Logging
	char * log_route;
	char * log_app_name;
	int log_in_console;
	int log_level_info;
} t_info_config;

int pedir_info_memoria();

// GETTERS
int entradas_tlb();
char * reemplazo_tlb();
int retardo_noop();
char * ip_memoria();
int puerto_memoria();
int puerto_escucha_dispatch();
int puerto_escucha_interrupt();

int get_puerto_escucha_dispatch();
int get_puerto_escucha_interrupt();

char * get_log_route();
char * get_log_app_name();
int get_log_in_console();
int get_log_level_info();

char * get_ip_memoria();
int get_puerto_memoria();

t_info_config config_guardada;

#endif /* CONFIGURACION_CPU_CONFIG_GUARDADA_H_ */
