#include "parser.h"

t_list *parsear_pseudocodigo() {

    bool debug = false;
	printf("\n Ingrese el path del archivo:  ");
    char* filePath = malloc(sizeof(char)*50);
	scanf("%s",filePath);

    char identificador[10]; //Pseudocodigo de instruccion
    uint32_t primer_operando;
	uint32_t segundo_operando;
    t_list *lista_instrucciones = list_create();

    FILE* pseudoFile = fopen(filePath,"r");
    fflush(stdout);
    while(!feof(pseudoFile)) {

        primer_operando = 0xFFFF;
		segundo_operando = 0xFFFF;
        fscanf(pseudoFile, "%s %u %u", identificador, &primer_operando, &segundo_operando);
        if(debug)puts("Lei del archivo");
        if(debug)loggear_trace("Lei instruccion %s", identificador);
        uint32_t id_instruccion_leida = convertir_identificador_a_enum(identificador);
        if(debug)loggear_trace("Lei instruccion de id %zu", id_instruccion_leida);
        if(id_instruccion_leida==1) {
            if(debug)puts("LEI NO OP");
            for (int i = 0; i < primer_operando; i++) {
            	t_instruccion* instruccion = crear_instruccion(id_instruccion_leida,0xFFFF,0xFFFF);
                if(debug)puts("ESCRIBO NO OP");
                if(instruccion) list_add(lista_instrucciones,instruccion);
            }
        }
        else {
        		t_instruccion* instruccion = crear_instruccion(id_instruccion_leida,primer_operando,segundo_operando);
                if(instruccion) list_add(lista_instrucciones,instruccion);
        }
    }
    fclose(pseudoFile);
    free(filePath);
    return lista_instrucciones;
}

t_instruccion* crear_instruccion (uint32_t identificador,uint32_t primer_operando,uint32_t segundo_operando) {
    t_instruccion* instruccion = malloc(sizeof(t_instruccion));
    if(identificador!=7){
        instruccion->id_instruccion = identificador;
        instruccion->primerOperando = primer_operando;
        instruccion->segundoOperando = segundo_operando;
        return instruccion;
    }
    loggear_warning("Error: Instruccion no reconocida");
    free(instruccion);
    return NULL;

} 