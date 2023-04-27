#include "cpu_main.h"

void cerrar_todo();

int main(int argc, char** argv) {

	int error = 0;

	error = iniciar_configuracion(argc, argv);
	if (error != STATUS_OK) {
		puts("Error en los argumentos\n");
		return EXIT_FAILURE;
	}

	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), LOG_LEVEL_TRACE);
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}

	error = pedir_info_memoria();
	if (error != STATUS_OK) {
		puts("Error al pedir datos a memoria\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}
	
	loggear_info("Se configura la TLB");
	iniciar_TLB();
	iniciar_mutex();


	int servidor = levantar_servidor();
	if (servidor != 0) {
		loggear_info("Server: %i", servidor);
		cerrar_todo();
		return EXIT_FAILURE;
	}
	puts("Levanté CPU!");


	sem_wait(&semaforo_detener_ejecucion);
	cerrar_todo();
	return EXIT_SUCCESS;
}

void cerrar_todo() {
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
	destruir_TLB();
	puts("TLB Destruida");
}
