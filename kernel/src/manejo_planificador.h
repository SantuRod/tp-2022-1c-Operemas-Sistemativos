
#ifndef MANEJO_PLANIFICADOR_H_
#define MANEJO_PLANIFICADOR_H_

#include <commons/collections/list.h>

#include <sys/mman.h>

#include "configuracion/kernel_configuracion.h"
#include <math.h>
#include <signal.h>

#include "../../shared/logger.h"
#include "../../shared/conexion.h"
#include "../../shared/protocolo.h"

#include "./conexion_consola/conexion_consola.h"
#include "./conexion_consola/coordinar_hilos.h"
#include "kernel_global.h"

//PLANIFICADOR CORTO PLAZO
void estado_exec();
void estado_ready();
void estado_blocked();
void iniciar_planificador_corto_plazo();
t_proceso * siguiente_a_ejecutar();

//PLANIFICADOR MEDIANO PLAZO
void iniciar_planificador_mediano_plazo();
void transicion_suspended_a_blocked();
void estado_suspend_ready();

//PLANIFICADOR LARGO PLAZO
void iniciar_planificador_largo_plazo();
void agregar_pcb_a_new(t_pcb *pcb,int socket_consola);
t_proceso *crear_proceso(t_pcb *pcbEnviada,int socket_consola);
void transicion_admitir_a_ready();
int recibir_tablas_de_pagina(int socket_fd,void *dest,uint32_t size);
void estado_exit();
t_proceso *proximo_a_ejecutar(char* algoritmo_de_planificacion);



#endif /* MANEJO_PLANIFICADOR_H_ */
