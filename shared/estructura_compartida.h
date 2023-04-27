#ifndef ESTRUCTURA_COMPARTIDA_H_
#define ESTRUCTURA_COMPARTIDA_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include <commons/collections/list.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include "protocolo.h"
#include "logger.h"

typedef enum {
	NUEVO,
	READY,
	EXEC,
	BLOCK,
	SUSP_BLOCK,
	SUSP_READY,
	FINISH
} t_estado_proceso;

typedef enum {
	BASURA,
	NO_OP,
	IO,
	READ,
	WRITE,
	COPY,
	EXIT
} t_id_instruccion;

typedef struct {
    uint32_t id;
    uint32_t tam_proceso; 
    t_list *instrucciones;
    uint32_t program_counter;
    uint32_t tabla_paginas; 
    float estimacion_rafaga;
} t_pcb;

typedef struct {
	t_id_instruccion id_instruccion;
	uint32_t primerOperando;
	uint32_t segundoOperando;
} t_instruccion;

typedef struct {
	uint32_t pid; //ID Proceso padre, usar ID o PCB entero?
	uint32_t tid; //ID Hilo

	//HEAP o no HEAP, esa es la cuestion
} t_hilo;

typedef struct {
	t_pcb *pcb;
	t_estado_proceso estado;
    uint32_t tiempo_cpu;//Tiempo que estuvo en CPU el proceso, se usa para calcular rafaga
	uint32_t tiempo_io;//Tiempo de espera de IO que devuelve la instruccion mediante CPU
	uint32_t tiempo_inicio_bloqueo;//Tiempo en el cual se inicia el bloqueo
	int socket;
} t_proceso;

typedef struct {
	uint32_t size;
	void* buffer;
} t_paquete;

char* convertir_estado_a_texto(t_estado_proceso estado);
t_id_instruccion convertir_identificador_a_enum(char* identificador);
void * serializar_pcb(t_pcb * pcb, int valor);
void* serializarInstrucciones (t_list* instrucciones); 
t_pcb* deserializarPCB (void* mensaje, int* valorIO);
t_pcb* generarMockPCB(bool mostrarLogs);

#endif /* ESTRUCTURA_COMPARTIDA_H_ */
