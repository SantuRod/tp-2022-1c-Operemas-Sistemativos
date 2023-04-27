#include "manejo_planificador.h"

void iniciar_planificador_mediano_plazo(){

 planificador_cola_suspend_blocked = queue_create();
 planificador_cola_suspend_ready = queue_create();
 pthread_create(&hilo_transicion_admitir_a_ready,NULL,(void*)estado_suspend_ready,NULL);
 pthread_detach(hilo_transicion_admitir_a_ready);
 sem_init(&mutex_cola_suspend_blocked,0,1);
 pthread_mutex_init(&mutex_cola_suspend_ready,NULL);
 sem_init(&sem_suspend_ready,0,0);

}

void transicion_suspended_a_blocked(t_proceso * proceso){  
  int socket_memoria = iniciar_conexion_memoria();
  uint32_t* paquete = malloc(sizeof(uint32_t));
  *paquete = proceso->pcb->tabla_paginas;
  enviar_mensaje_protocolo(socket_memoria, SUSPENSION_K_M, sizeof(uint32_t),paquete);
  free(paquete);
  int res = recibir_mensaje_memoria(socket_memoria);
  loggear_info("Resultado de suspension de proceso en MEMORIA [%d]: [%d]", proceso->pcb->id, res);
  close(socket_memoria);
  sem_wait(&mutex_cola_suspend_blocked);
  proceso->estado=SUSP_BLOCK;
  queue_push(planificador_cola_suspend_blocked,proceso);
  loggear_info("==================================================================");
  loggear_info("PID = [%d] ingresa al estado SUSPEND-BLOCKED",proceso->pcb->id);
  loggear_info("==================================================================");
  sem_post(&sem_grado_multiprogramacion);
  sem_post(&mutex_cola_suspend_blocked);
 }

void estado_suspend_ready(){
  while(1){
    sem_wait(&sem_suspend_ready);
    sem_wait(&mutex_cola_suspend_blocked);
    t_proceso *proceso = queue_pop(planificador_cola_suspend_blocked);
    sem_post(&mutex_cola_suspend_blocked);
     
    loggear_info("==================================================================");
    loggear_info("PID = [%d] Sale del estado SUSPENDED-BLOCKED ",proceso->pcb->id);
    loggear_info("==================================================================");
    
    // --------------------------------------
    pthread_mutex_lock(&mutex_cola_suspend_ready);
    proceso->estado=SUSP_READY;
    loggear_info("=================================================");
    loggear_info("PID = [%d] Entrando al estado SUSPEND-READY",proceso->pcb->id);
    loggear_info("=================================================");
    queue_push(planificador_cola_suspend_ready,proceso);
    pthread_mutex_unlock(&mutex_cola_suspend_ready);
    

   sem_post(&sem_admitir_proceso);
  }

}