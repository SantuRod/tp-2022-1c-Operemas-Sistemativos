#include "temporizador.h"

void temporizador();

//inicializa en un hilo separado.
uint32_t tiempo=0;
pthread_mutex_t mutex_timer;
pthread_t th_timer;

//corro el tiempo en un hilo 
void iniciar_timer(){
 pthread_mutex_init(&mutex_timer,NULL);
 pthread_create(&th_timer,NULL,(void*)temporizador,NULL);
 pthread_detach(th_timer);

}

//aca vou incrementando el tiempo
void temporizador(){
  while(1){
   sleep(1); // 1 segundo
   pthread_mutex_lock(&mutex_timer);
   tiempo++;
   pthread_mutex_unlock(&mutex_timer);
  }
}

uint32_t get_tiempo_actual(){
  pthread_mutex_lock(&mutex_timer);
  uint32_t tiempo_actual = tiempo * 1000;
  pthread_mutex_unlock(&mutex_timer);
  return tiempo_actual;
}

