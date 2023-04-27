#ifndef KERNEL_GLOBAL_H
#define KERNEL_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>

// DATOS GENERALES
sem_t sem_grado_multiprogramacion;

// PLANIFICADOR CORTO PLAZO
t_list *planificador_cola_ready;
t_queue *planificador_cola_blocked;
t_queue *planificador_cola_exec;
sem_t mutex_cola_blocked;
sem_t mutex_exec;
sem_t mutex_cola_ready;
sem_t sem_estado_blocked;
sem_t sem_estado_ready;
sem_t sem_execute;
sem_t sem_ready;
sem_t sem_suspend_ready;
sem_t sem_CPU_Ejecutando;
sem_t sem_desalojar;
sem_t sem_sort_SRT;
bool cpu_ejecutando;
pthread_t hilo_ready;
pthread_t hilo_exec;
pthread_t hilo_blocked;
sem_t mutex_execucion;

sem_t sem_ejecucion;
sem_t sem_ejecucion1;

// PLANIFICADOR MEDIANO PLAZO Y SEMAFOROS Y HILOS 
t_queue *planificador_cola_suspend_blocked;
t_queue *planificador_cola_suspend_ready;
sem_t mutex_cola_suspend_blocked;
pthread_mutex_t mutex_cola_suspend_ready;
pthread_t hilo_transicion_admitir_a_ready;
pthread_t hilo_estado_exit;


//PLANIFICADOR LARGO PLAZO
t_queue* planificador_cola_new;
sem_t mutex_cola_new;
t_queue* planificador_cola_exit;
sem_t mutex_cola_exit;
sem_t sem_admitir_proceso;
sem_t sem_cola_exit;






#endif /* KERNEL_GLOBAL_H_ */