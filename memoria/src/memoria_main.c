#include "memoria_main.h"

void cerrar_todo();

int main(int argc, char** argv) {

	int error = iniciar_configuracion(argc, argv);
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

	inicializar_memoria();

	int servidor = levantar_servidor();
	if (servidor != 0) {
		loggear_info("Server: %i", servidor);
		cerrar_todo();
		return EXIT_FAILURE;
	}
	puts("Levanté Memoria!");

	/** cuando pasa a ready, le doy (cant_marcos_x_proceso) marcos de la memoria
	 * 
	 * a medida que me va pidiendo hago el algoritmo de reemplazo
	 * 
	 * escribo/leo lo que necesito lo que necesito
	 *
	 * Proceso de escribir/leer: 
	 *
	 * Me pide indice pagina 2 (direccion 1 = indice de la lista de tablas nivel 2, Es decir, si le digo 2 me trae tabla en la 3ra entrada de la lista nivel 1)
	 * Me pide marco (direccion 2 = indice de la pagina es decir, si le digo 24, es la pagina en la 25va entrada de la lista de nivel 2)
	 * 	* Si tiene marco, lo devuelvo
	 *  * Si no tiene marco, reemplazo y despues lo devuelvo
	 * Me tira donde escribir
	 *  
	*/
	cerrar_todo();
	return EXIT_SUCCESS;
}

void cerrar_todo() {
	cerrar_conexiones(true);
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
	destruir_memoria();
	puts("Destruida memoria");
}


