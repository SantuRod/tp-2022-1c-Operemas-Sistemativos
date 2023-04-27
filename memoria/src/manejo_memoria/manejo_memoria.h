
#ifndef MANEJO_MEMORIA_H_
#define MANEJO_MEMORIA_H_


#include <unistd.h>

#include "../inicializacion/inicializacion.h"
#include "../../../shared/logger.h"

/**
 * @NAME: leer_memoria
 * @DESC: lee el valor de {un uint32_t} y lo devuelve como void*  
 * @RETURN: el valor como un puntero a algo de lo que encontro en ese espacio de la memoria
 */
uint32_t leer_memoria(int direccion_logica);

/**
 * @NAME: escribir_memoria
 * @DESC: escribe en la direccion ingresada el valor de {un uint32_t} 
 * @RETURN: el valor como un puntero a algo de lo que encontro en ese espacio de la memoria
 */
bool escribir_memoria(int direccion_logica, uint32_t data);

void loggear_memoria();
void randomizar_memoria();

int cargar_pagina_en_memoria(int indice, int nro_pagina, int marco);
int meter_pagina_en_memoria(void* pagina, int direccion);
int meter_proceso_en_memoria(int indice);
void guardar_en_swap(int indice, int nro_pagina , int marco);
void leer_swap(int direccion, int indice);
int borrar_swap(int indice);
void loggear_ctrl_memoria();
#endif /* MANEJO_MEMORIA_H_ */
