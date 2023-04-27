#include "consola_main.h"

void cerrar_todo();
int flagDebug;

int main(int argc, char** argv) {

	flagDebug = 1;

	int error = 0;
	//INICIAR CFG
	error = iniciar_configuracion(argc, argv);
	if (error != STATUS_OK) {
		puts("Error en los argumentos\n");
		return EXIT_FAILURE;
	}


	//INICIAR CONSOLA
	error = init_mutex_log(get_log_route(), get_log_app_name(), get_log_in_console(), LOG_LEVEL_TRACE);
	if (error != STATUS_OK) {
		puts("Error al crear el logger\n");
		cerrar_todo();
		return EXIT_FAILURE;
	}
	
	puts("Levanté consola!");

		//PARSEAR PSEUDOCODIGO Y LEER TAM CONSOLA
		t_list *instrucciones = parsear_pseudocodigo();
		uint32_t tam_consola;
		printf("\n Ingrese el tamano del proceso:  ");
		scanf("%d",&tam_consola);

		puts("\n\nLevanté socket!");
		int socket_kernel = iniciar_conexion_kernel();

		if (socket_kernel > 0) {//Conexion exitosa

			if(flagDebug != -1) {

				size_t* tam_paquete = malloc(sizeof(uint32_t));

				void* paquete = serializarInstruccionesConsola(instrucciones,tam_consola, tam_paquete);
				
				loggear_info("Enviando instrucciones a kernel con tamaño: %zu", *tam_paquete);

				if( enviar_mensaje_protocolo(socket_kernel, MENSAJE_CO_K , *tam_paquete, paquete ) < 0 ) {
					loggear_error("Error al enviar");
				}

				list_destroy(instrucciones);
				free(tam_paquete);
				free(paquete);
				loggear_info("Espero mensaje de cierre de kernel");
				 int ret = recibir_mensaje_kernel(socket_kernel);
				 if(ret!=0 && ret!=-23) loggear_error("Ocurrio un error el recibir el mensaje de cierre de kernel, codigo %d",ret);

			}
		}
		else {
			loggear_info("Fallo la conexion al socket");
			kernel_cerrar_conexiones(true);
			cerrar_todo();
			return EXIT_FAILURE;
		}
	cerrar_todo();
	return EXIT_SUCCESS;
}

void cerrar_todo() {
	destroy_configuracion();
	puts("Destruido configuraciones");
	destroy_log();
	puts("Destruido logs");
}
