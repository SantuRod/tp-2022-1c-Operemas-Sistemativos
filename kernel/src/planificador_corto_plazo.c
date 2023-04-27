#include "manejo_planificador.h"
#include "temporizador.h"

uint32_t diferencia_absoluta(uint32_t tiempo_1,uint32_t tiempo_2);
void io(uint32_t timer);
double calcular_estimacion_rafaga(uint32_t tiempo_ejecucion,double tiempo_estimado);
t_proceso *proceso_en_ejecucion;

void iniciar_planificador_corto_plazo(){
 planificador_cola_ready = list_create();
 planificador_cola_exec = queue_create();
 planificador_cola_blocked=queue_create();
 sem_init(&mutex_exec,0,1);
 sem_init(&mutex_cola_ready,0,1);
 sem_init(&mutex_cola_blocked,0,1);
 sem_init(&mutex_execucion,0,1);

 sem_init(&sem_ejecucion,0,1);
 sem_init(&sem_ejecucion1,0,0);
 sem_init(&sem_estado_ready,0,0);
 sem_init(&sem_estado_blocked,0,0);
 sem_init(&sem_execute,0,0);
 sem_init(&sem_CPU_Ejecutando,0,0);
 sem_init(&sem_desalojar,0,1);
 sem_init(&sem_sort_SRT,0,0);
 
 pthread_create(&hilo_ready,NULL,(void *)estado_ready,NULL);
 pthread_create(&hilo_exec,NULL,(void *)estado_exec,NULL);
 pthread_create(&hilo_blocked,NULL,(void*)estado_blocked,NULL);
 pthread_detach(hilo_ready);
 pthread_detach(hilo_exec);
 pthread_detach(hilo_blocked);

 iniciar_timer();
 cpu_ejecutando = false;
}

void estado_ready(){	
  while(1){
    sem_wait(&sem_estado_ready);	
 	sem_wait(&sem_desalojar);
	t_proceso* proceso=siguiente_a_ejecutar();
	sem_wait(&mutex_execucion);
	queue_push(planificador_cola_exec,proceso);
	sem_post(&mutex_execucion);
	sem_post(&sem_execute);
  }
}


void estado_exec(){

  while(1){
	sem_wait(&sem_execute);
	sem_wait(&mutex_execucion);
	t_proceso* proceso = queue_pop(planificador_cola_exec);
	proceso->estado=EXEC;
	cpu_ejecutando=true;
	loggear_info("==================================================================");
    loggear_info("PID = [%d], Ingresa al estado EXECUTE", proceso->pcb->id);
	loggear_info("==================================================================");
	sem_post(&mutex_execucion);

    size_t tamPaquete = sizeof(t_instruccion) * list_size(proceso->pcb->instrucciones) +  sizeof(uint32_t) + sizeof(uint32_t) * 4 + sizeof(float) + sizeof(int);	
    void* paquete=serializar_pcb(proceso->pcb,0);
    int* valor_io = malloc(sizeof(int));
	destructorPCB(proceso->pcb);
	uint32_t tiempo_inicio_a_cpu = get_tiempo_actual();

	int conexion_dispatch = iniciar_conexion_cpu();

	if( enviar_mensaje_protocolo(conexion_dispatch, MENSAJE_DISPATCH_K_CPU , tamPaquete, paquete ) < 0 ){
	 	loggear_error("Error al enviar dispatch a CPU");
		 free(paquete);
    }
	else{
        t_prot_mensaje* mensaje= recibir_mensaje_protocolo(conexion_dispatch);
		proceso->pcb = deserializarPCB(mensaje->payload,valor_io);
		loggear_info("Recibi deserializacion");
		proceso->tiempo_cpu += get_tiempo_actual() - tiempo_inicio_a_cpu;
		loggear_debug("Recibi de CPU PID = [%zu]",proceso->pcb->id);
		loggear_debug("TIEMPO QUE ESTUVO EN CPU: %zu", proceso->tiempo_cpu);
		free(paquete);
		destruir_mensaje(mensaje);
	}

	sem_wait(&mutex_execucion);
	cpu_ejecutando = false;
	sem_post(&mutex_execucion);
    
    close(conexion_dispatch);
	//Interpretar la instruccion de la CPU
	if(*valor_io >=0){   //CPU devuelve IO, lo mandamos a blocked
		sem_wait(&mutex_cola_blocked);
		proceso->tiempo_io=*valor_io;
		proceso->estado=BLOCK;
		proceso->tiempo_inicio_bloqueo = get_tiempo_actual();
		proceso->pcb->estimacion_rafaga = calcular_estimacion_rafaga(proceso->tiempo_cpu, proceso->pcb->estimacion_rafaga);
		proceso->tiempo_cpu = 0;
		queue_push(planificador_cola_blocked,proceso);
		sem_post(&mutex_cola_blocked);
		sem_post(&sem_estado_blocked);
	}
	else if(*valor_io==-1) { //CPU devuelve EXIT, lo mandamos a exit
		sem_wait(&mutex_cola_exit);
		proceso->estado=EXIT;
		queue_push(planificador_cola_exit,proceso);
		sem_post(&mutex_cola_exit);
		sem_post(&sem_cola_exit);
	}
	else if(*valor_io==-2) { //CPU interrumpida por SRT
		sem_wait(&mutex_cola_ready);
		proceso->estado=READY;
		list_add(planificador_cola_ready,proceso);
		sem_post(&mutex_cola_ready);
		sem_post(&sem_estado_ready);
	
	}
	else {
		loggear_error("Error: Codigo desconocido de IO");
	}
	free(valor_io);
	sem_post(&sem_desalojar);

    }	

}

void estado_blocked(){
	while(1){

     sem_wait(&sem_estado_blocked);
	 sem_wait(&mutex_cola_blocked);
	 t_proceso* proceso=queue_pop(planificador_cola_blocked);
	 proceso->estado=BLOCK;
	 sem_post(&mutex_cola_blocked);
     
	 loggear_info("==================================================================");
	 loggear_info(" PID = [%d], proceso ingresando al estado BLOCKED",proceso->pcb->id);
	 loggear_info("==================================================================");
	 uint32_t tiempo_bloqueado = get_tiempo_actual() - proceso->tiempo_inicio_bloqueo;
	 loggear_info("Tiempo actual: %zu ms",get_tiempo_actual());
	 loggear_info("Tiempo inicio bloqueo: %zu ms",proceso->tiempo_inicio_bloqueo);
	 loggear_info("Tiempo bloqueado: %zu ms",tiempo_bloqueado);
	 loggear_info("Tiempo maximo bloqueado: %zu ms", get_tiempo_maximo_bloqueado());

	 if(tiempo_bloqueado > get_tiempo_maximo_bloqueado()){
		 //loggear_trace("if");
		 loggear_trace("tiempo bloqueado: %zu", tiempo_bloqueado);
		 transicion_suspended_a_blocked(proceso);
		 io(proceso->tiempo_io);
		 sem_post(&sem_suspend_ready);
	 }
	 else if(tiempo_bloqueado + proceso->tiempo_io > get_tiempo_maximo_bloqueado()){
		 //loggear_trace("Else if");
		 uint32_t tiempo_io_antes_de_suspender = get_tiempo_maximo_bloqueado() - tiempo_bloqueado;
		 loggear_info("proceso->tiempo_io: %zu",proceso->tiempo_io);
		 loggear_info("Tiempo IO Pre Susp: %zu ms",tiempo_io_antes_de_suspender);
		 io(tiempo_io_antes_de_suspender);
		 transicion_suspended_a_blocked(proceso);
		 io(diferencia_absoluta(tiempo_io_antes_de_suspender,proceso->tiempo_io));
		 sem_post(&sem_suspend_ready);
	 }
	 else{
		 io(proceso->tiempo_io);
		 sem_wait(&mutex_cola_ready);
		 list_add(planificador_cola_ready,proceso);
		 sem_post(&mutex_cola_ready);
		 loggear_info("==================================================================");
         loggear_info("PID = [%d] ingresa al estado READY desde estado BLOCKED",proceso->pcb->id);
         loggear_info("==================================================================");
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
}

t_proceso * siguiente_a_ejecutar(){
	char* algoritmo = config_guardada.algoritmo_planificacion;
   t_proceso*proceso=NULL;

   bool menor_rafaga (void*p1,void*p2){
	   uint32_t estimacion_p1=((t_proceso*)p1)->pcb->estimacion_rafaga - ((t_proceso*)p1)->tiempo_cpu;
	   uint32_t estimacion_p2=((t_proceso*)p2)->pcb->estimacion_rafaga - ((t_proceso*)p2)->tiempo_cpu;
	   return estimacion_p1 <= estimacion_p2;
   }
   
   sem_wait(&mutex_cola_ready);
   if(string_equals_ignore_case(algoritmo,"SRT")){
	   loggear_warning("  ALGORITMO SRT, REPLANIFICANDO...");
	   list_sort(planificador_cola_ready,menor_rafaga);
   }

   proceso = (t_proceso*)list_remove(planificador_cola_ready,0);
   sem_post(&mutex_cola_ready);
   return proceso;

}

void io(uint32_t tiempo){
	loggear_trace("El proceso será bloqueado por %zu ms",tiempo);
	usleep(tiempo*1000);
	loggear_trace("El proceso dejó de estar bloqueado.");
}

uint32_t diferencia_absoluta(uint32_t tiempo_1, uint32_t tiempo_2){
	return tiempo_1 > tiempo_2 ? tiempo_1 - tiempo_2 : tiempo_2 - tiempo_1;
}

double calcular_estimacion_rafaga(uint32_t tiempo_ejecucion, double tiempo_estimado){
	loggear_trace("ESTIMACION: %f",config_guardada.alfa * tiempo_ejecucion + (1 - config_guardada.alfa) * tiempo_estimado);
	return config_guardada.alfa * tiempo_ejecucion + (1 - config_guardada.alfa) * tiempo_estimado;
}

void eliminar_pcb(t_pcb* pcb){
	list_destroy_and_destroy_elements(pcb->instrucciones,free);
	free(pcb);
}