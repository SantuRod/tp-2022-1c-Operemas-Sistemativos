#ifndef PARSEADOR_H_
#define PARSEADOR_H_

#include <ctype.h>
#include<string.h>
#include "../../shared/estructura_compartida.h"

t_list *parsear_pseudocodigo();
t_instruccion* crear_instruccion (uint32_t identificador,uint32_t primer_operando,uint32_t segundo_operando);

#endif
