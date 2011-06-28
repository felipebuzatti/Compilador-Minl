#ifndef PROCEDIMENTOS_H_INCLUDED
#define PROCEDIMENTOS_H_INCLUDED

#include "estruturas_semanticas.h"


typedef struct str_param
{
	char nome[TAM_IDENT+1];
	int tipo;
	struct str_param * prox;
} tipo_param;

typedef struct
{
	tipo_param * primeiro;
	tipo_param * ultimo;
	int endereco;
	int num_param;
} tipo_lista_param;

tipo_param * cria_param(int tipo, char * nome);
tipo_lista_param * inicializa_lista_param();
tipo_lista_param * insere_param(tipo_lista_param * lista_param, tipo_param * param);
void libera_lista_param(tipo_lista_param * lista_param);

////////////////////////////////////////////////////////////////////////////////

tipo_lista_param * formal_list1(tipo_param * param);
int proc_header(char * nome_proc, tipo_lista_param * lista_param);
tipo_lista_comandos * proc_stmt1(char * identificador);
tipo_lista_comandos * proc_stmt2(char * identificador, tipo_lista_comandos * lista_comandos);
tipo_lista_comandos * proc_decl(tipo_lista_param * lista_param, tipo_lista_comandos * comandos_proc);


#endif /* PROCEDIMENTOS_H_INCLUDED */
