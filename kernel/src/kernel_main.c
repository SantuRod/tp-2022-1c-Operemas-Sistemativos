#include "kernel_main.h"

void cerrar_todo();

void destructorInstrucciones();

sem_t mutex_execute;

int main(int argc, char** argv) {

	int error = 0;
	error = iniciar_configuracion(argc, argv);
	if (error != STATUS_OK) {
		puts("Error en los argumentos\n");
		return EXIT_FAILURE;
	}

	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), get_log_level_info());
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}
 

    inicializar_planificadores();

	int servidor = levantar_servidor();
	if (servidor != 0) {
		loggear_info("Error de server: %i", servidor);
		cerrar_todo();
		return EXIT_FAILURE;
	}

	puts("Levanté Kernel!");

    sem_wait(&sem_ejecucion1);

	loggear_warning("Cerrando kernel");
	cerrar_todo();
	return EXIT_SUCCESS;
}

void inicializar_planificadores(){

	iniciar_planificador_largo_plazo();
	iniciar_planificador_mediano_plazo();
	iniciar_planificador_corto_plazo();

}

void destructorPCB(t_pcb* pcb) {
	list_destroy_and_destroy_elements(pcb->instrucciones,destruirInstruccion);
	free(pcb);
}

void destruirInstruccion(void* instruccion) {
	free((t_instruccion*)instruccion);
}

void cerrar_todo(){
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
}
