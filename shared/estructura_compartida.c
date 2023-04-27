
#include "estructura_compartida.h"

const size_t SIZE_ID_INSTRUCCION = sizeof(uint32_t);
const size_t SIZE_PRIMER_OPERANDO = sizeof(uint32_t);
const size_t SIZE_SEGUNDO_OPERANDO = sizeof(t_id_instruccion);

char* convertir_estado_a_texto(t_estado_proceso estado){
   switch (estado) {
	   case NUEVO: return "NEW";
	   case READY: return "READY";
	   case EXEC: return "EXEC";
	   case BLOCK: return "BLOCK I/O";
	   case SUSP_BLOCK: return "SUSP_BLOCK I/O";
	   case SUSP_READY: return "SUSP_READY I/O";
	   case FINISH: return "FINISH";
   }
   return "No tiene estado conocido";
}
 
uint32_t convertir_identificador_a_enum(char* identificador) {
	if(string_equals_ignore_case(identificador,"NO_OP")) return 1;
	if(string_equals_ignore_case(identificador,"I/O")) return 2;
	if(string_equals_ignore_case(identificador,"READ")) return 3;
	if(string_equals_ignore_case(identificador,"WRITE")) return 4;
	if(string_equals_ignore_case(identificador,"COPY")) return 5;
	if(string_equals_ignore_case(identificador,"EXIT")) return 6;
	return -1;
}

//Generar PCB para testing
t_pcb* generarMockPCB() {
	t_list * instrucciones = list_create();
    t_pcb* pcbInicial = malloc(sizeof(t_pcb));
        pcbInicial->id = rand();
        for(int i = 0;i<5;i++) {
            t_instruccion* instruccion = malloc(sizeof(t_instruccion));
            instruccion->id_instruccion = 0;
            instruccion->primerOperando = 5;
            instruccion->segundoOperando = 1;
			list_add(instrucciones, instruccion);
        }
    pcbInicial->program_counter = 0;
    pcbInicial->tabla_paginas = 0;
    pcbInicial->estimacion_rafaga = 0;
	pcbInicial->instrucciones = instrucciones;
    return pcbInicial;
}


//----------------------------------------------------------SERIALIZACIONES---------------------------------------------------------- 
void * serializar_pcb(t_pcb * pcb, int valor){
	void * instrucciones = serializarInstrucciones(pcb->instrucciones);
	size_t tamanio_buffer = sizeof(t_instruccion) * list_size(pcb->instrucciones) +  sizeof(uint32_t) + sizeof(uint32_t) * 4 + sizeof(float) + sizeof(int);
	void * paquete = malloc(tamanio_buffer);
	size_t offset = 0;

	memcpy(paquete + offset,&pcb->id,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(paquete + offset,&pcb->tam_proceso,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(paquete + offset,&pcb->program_counter,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(paquete + offset, &pcb->tabla_paginas,sizeof(uint32_t));
	offset+=sizeof(uint32_t);
	memcpy(paquete + offset,&valor,sizeof(int));
	offset+=sizeof(int);
	memcpy(paquete + offset,&pcb->estimacion_rafaga,sizeof(float));
	offset+=sizeof(float);
	memcpy(paquete+offset,instrucciones, sizeof(t_instruccion) * list_size(pcb->instrucciones) + sizeof(uint32_t));
	offset+=sizeof(t_instruccion) * list_size(pcb->instrucciones) + sizeof(uint32_t);
	free(instrucciones);
	return paquete;
}

void* serializarInstrucciones (t_list* instrucciones) {
	size_t tamanio_buffer = sizeof(t_instruccion) * list_size(instrucciones) +  sizeof(uint32_t);
	void* paquete = malloc(tamanio_buffer);
	size_t offset = 0;
	uint32_t cantInstrucciones;
	cantInstrucciones =  list_size(instrucciones);
	memcpy(paquete + offset, &cantInstrucciones, sizeof(uint32_t));   //CANT INSTRUCCIONES OK
	offset+=sizeof(uint32_t);

	for(int i=0;i<list_size(instrucciones);i++) {
		t_instruccion* instruccion = list_get(instrucciones,i);
		memcpy(paquete + offset,&instruccion->id_instruccion,sizeof(uint32_t));
		offset+=sizeof(uint32_t);;
		memcpy(paquete + offset,&instruccion->primerOperando,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(paquete + offset,&instruccion->segundoOperando,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
	}

	return paquete;	
}

//----------------------------------------------------------DESERIALIZACIONES---------------------------------------------------------- 

t_pcb* deserializarPCB (void* mensaje, int* valorIO) {
		size_t offset = 0;
		t_pcb* pcbInicial = malloc(sizeof(t_pcb));
		memcpy(&pcbInicial->id,mensaje+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(&pcbInicial->tam_proceso,mensaje+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(&pcbInicial->program_counter,mensaje+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(&pcbInicial->tabla_paginas,mensaje+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t);
		memcpy(valorIO,mensaje+offset,sizeof(int));
		offset+=sizeof(int);
		void* aux = malloc(sizeof(float));
		memcpy(aux,mensaje+offset,sizeof(uint32_t));
		pcbInicial->estimacion_rafaga = *(float*)aux;
		offset+=sizeof(float);
 
		uint32_t* cantInstrucciones = malloc(sizeof(uint32_t));
		memcpy(cantInstrucciones,mensaje+offset,sizeof(uint32_t));
		offset+=sizeof(uint32_t); 
		pcbInicial->instrucciones = list_create();
 
		for(int i=0;i<*cantInstrucciones;i++) {
			t_instruccion* instruccion = malloc(sizeof(t_instruccion));
			memcpy(&instruccion->id_instruccion, mensaje+offset,sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			memcpy(&instruccion->primerOperando, mensaje+offset,sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			memcpy(&instruccion->segundoOperando, mensaje+offset,sizeof(uint32_t));
			offset+=sizeof(uint32_t);
			list_add(pcbInicial->instrucciones, instruccion);
		}
		free(cantInstrucciones);
		free(aux);
	return pcbInicial;
}