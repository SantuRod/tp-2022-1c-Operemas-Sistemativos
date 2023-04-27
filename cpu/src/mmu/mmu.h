
#ifndef MMU_H_
#define MMU_H_


#include <sys/mman.h>
#include <commons/collections/list.h>

#include "../configuracion/cpu_configuracion.h"	// Carga del archivo de configuracion
#include <readline/readline.h>
#include <signal.h>
#include "../mmu/mmu.h"

#include "../conexion_kernel/conexion_kernel.h"
#include "../conexion_kernel/coordinar_hilos.h"
#include "../conexion_memoria/manejar_mensajes_memoria.h"

#include "../../../shared/logger.h"			// Mutex para escribir en consola y en archivo de log
#include "../../../shared/conexion.h"			// Crear y conectarse a servidor
#include "../../../shared/protocolo.h"			// Enviar mensajes a través de sockets
#include "../../../shared/manejo_hilos.h"		// Manejar cierre de hilos y loggear en caso de error

typedef struct{
    uint32_t pagina;
    uint32_t marco;
} t_tlb;

typedef enum {
    READ_OPER,
    WRITE_OPER
} t_tipo_oper;

//INIT, CLEAN Y DESTROY
void iniciar_TLB();
void limpiar_TLB(uint32_t pcb_id_ciclo);
void destruir_TLB();

//ACCIONES A MEMORIA
uint32_t peticion_READ_MMU(uint32_t dir_logica, uint32_t tabla_paginas);
void peticion_WRITE_MMU(uint32_t dir_logica, uint32_t tabla_paginas , uint32_t valor_a_escribir);
uint32_t peticion_memoria(uint32_t dir_logica, uint32_t tabla_paginas , uint32_t tipo_operacion, uint32_t valor_a_escribir);

// TRADUCCIONES DE DIRECCION LOGICA A FISICA
uint32_t calcular_numero_pagina(uint32_t dir_logica);
uint32_t calcular_entrada_tabla_1er_nivel(uint32_t numero_pagina);
uint32_t calcular_entrada_tabla_2do_nivel(uint32_t numero_pagina);
uint32_t calcular_desplazamiento(uint32_t dir_logica, uint32_t numero_pagina);

//ACCIONES TLB
void agregar_a_tlb(uint32_t numero_pagina, uint32_t marco);
void ordenar_tlb(t_tlb * tlb_elem, uint32_t numero_pagina);
int buscar_en_TLB(uint32_t numero_pagina);
#endif /* MMU_H_ */