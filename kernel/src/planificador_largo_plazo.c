#include "manejo_planificador.h"

void iniciar_planificador_largo_plazo(){
    planificador_cola_new = queue_create();
    planificador_cola_exit = queue_create();
    sem_init(&mutex_cola_new,0,1);
    sem_init(&mutex_cola_exit,0,1); 
    sem_init(&sem_grado_multiprogramacion,0,get_grado_multiprogramacion());
    sem_init(&sem_admitir_proceso,0,0); 
    sem_init(&sem_cola_exit,0,0);
    pthread_create(&hilo_transicion_admitir_a_ready,NULL,(void *)transicion_admitir_a_ready,NULL);
    pthread_create(&hilo_estado_exit,NULL,(void*) estado_exit,NULL);
    pthread_detach(hilo_transicion_admitir_a_ready);
    pthread_detach(hilo_estado_exit);
    
}

t_proceso *crear_proceso(t_pcb *pcbEnviada,int socket_consola){
    t_proceso *proceso=malloc(sizeof(t_proceso));
    proceso->pcb=pcbEnviada;
    proceso->socket=socket_consola;
    proceso->estado=NUEVO;
    proceso->tiempo_io=0;
    proceso->tiempo_inicio_bloqueo=0;
    proceso->tiempo_cpu=0;
     
    return proceso;
}

void agregar_pcb_a_new(t_pcb *pcbEnviada,int socket_consola){
    t_proceso *proceso=crear_proceso(pcbEnviada,socket_consola);
    sem_wait(&mutex_cola_new);
    queue_push(planificador_cola_new,proceso);
    sem_post(&mutex_cola_new);
    loggear_info("==================================================================");
    loggear_info("PID = [%d] ingresa a NEW",proceso->pcb->id);
    loggear_info("==================================================================");
    sem_post(&sem_admitir_proceso);  
} 

//corre en un hilo
void transicion_admitir_a_ready(){
    
    while(1){
        sem_wait(&sem_admitir_proceso);   
        sem_wait(&sem_grado_multiprogramacion);
        t_proceso* proceso;
        pthread_mutex_lock(&mutex_cola_suspend_ready);
        if(!queue_is_empty(planificador_cola_suspend_ready)){
            proceso=queue_pop(planificador_cola_suspend_ready);
            pthread_mutex_unlock(&mutex_cola_suspend_ready);
            loggear_info("==================================================================");
            loggear_info("PID = [%d] ingresa al estado READY desde estado SUSPEND_READY",proceso->pcb->id);
            loggear_info("==================================================================");
        }//else if(){
        //}
        else{
            pthread_mutex_unlock(&mutex_cola_suspend_ready);
            sem_wait(&mutex_cola_new);
            proceso=queue_pop(planificador_cola_new);
            sem_post(&mutex_cola_new);
            loggear_info("==================================================================");
            loggear_info("PID = [%d] ingresa al estado READY viniendo del estado NEW",proceso->pcb->id);
            loggear_info("==================================================================");
        }

        int socket_memoria = iniciar_conexion_memoria();
        uint32_t* paquete = malloc(sizeof(uint32_t));
        *paquete = proceso->pcb->tabla_paginas;
        enviar_mensaje_protocolo(socket_memoria, RESERVAR_MARCOS_K_M, sizeof(uint32_t),paquete);
        free(paquete);
        int res = recibir_mensaje_memoria(socket_memoria);
        loggear_info("Resultado reserva marcos para proceso [%d] exit: [%d]", proceso->pcb->id, res!=-24?res:0);
        close(socket_memoria);
        
        sem_wait(&mutex_cola_ready);
        proceso->estado=READY;
        list_add(planificador_cola_ready,proceso);
        sem_post(&mutex_cola_ready);

        //En la transición hacia ready, se envía la interrupción para desalojar el proceso.
        if(string_equals_ignore_case(get_algoritmo_planificacion(),"SRT")){
	        sem_wait(&mutex_execucion);
		    if(cpu_ejecutando) {
		        int	conexion_interrupt = iniciar_conexion_cpu_interrupt();
			    enviar_mensaje_protocolo(conexion_interrupt, MENSAJE_INTERRUPT_K_CPU , 0, NULL);
			    loggear_warning("                   ALGORITMO SRT  ");
			    loggear_warning("                ENVIANDO INTERRUPCION                 "); 
			    close(conexion_interrupt);
		    }
		sem_post(&mutex_execucion);
	    }
        sem_post(&sem_estado_ready);
    }
}


void estado_exit(){

    while(1){
        sem_wait(&sem_cola_exit);
        sem_wait(&mutex_cola_exit);
        t_proceso *proceso=queue_pop(planificador_cola_exit);
        sem_post(&mutex_cola_exit);
        loggear_trace("==================================================================");
        loggear_trace("PID = [%d] ingresa al estado EXIT", proceso->pcb->id);
        loggear_trace("==================================================================");
        
        int socket_memoria = iniciar_conexion_memoria();
        uint32_t* paquete = malloc(sizeof(uint32_t));
        *paquete = proceso->pcb->tabla_paginas;
        loggear_info("Aviso a memoria que me matan");
        enviar_mensaje_protocolo(socket_memoria, FINALIZACION_K_M, sizeof(uint32_t),paquete);

        int res = recibir_mensaje_memoria(socket_memoria);
        loggear_info("Resultado finalizacion proceso [%d] exit: [%d]", proceso->pcb->id, res);
        close(socket_memoria);
        
        enviar_mensaje_protocolo(proceso->socket,DESCONEXION_KERNEL_K_CO,0,NULL);
        close(proceso->socket);
        sem_post(&sem_grado_multiprogramacion);
    }
}


