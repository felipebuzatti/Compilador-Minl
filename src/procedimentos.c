#include "procedimentos.h"
#include "estruturas_semanticas.h"
#include "floresta.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern tipo_floresta * tab_simbolos;

tipo_param * cria_param(int tipo, char * nome)
{
	tipo_param * param = (tipo_param*)malloc(sizeof(tipo_param));
	strcpy(param->nome, nome);
	param->tipo = tipo;
	param->prox = NULL;
	return param;
}

tipo_lista_param * inicializa_lista_param()
{
	tipo_lista_param * lista_param = (tipo_lista_param*)malloc(sizeof(tipo_lista_param));
	lista_param->primeiro = NULL;
	lista_param->ultimo = NULL;
	lista_param->endereco = 0;
	lista_param->num_param = 0;
	return lista_param;
}

tipo_lista_param * insere_param(tipo_lista_param * lista_param, tipo_param * param)
{
	if (lista_param->ultimo == NULL)
	{
		lista_param->primeiro = param;
	}
	else
	{
		lista_param->ultimo->prox = param;
	}
		
	lista_param->ultimo = param;
	lista_param->num_param+=1;
	
	return lista_param;
}

void libera_lista_param(tipo_lista_param * lista_param)
{
	while (lista_param->primeiro != NULL)
	{
		tipo_param * aux = lista_param->primeiro;
		lista_param->primeiro = aux->prox;
		free(aux);
	}
}

////////////////////////////////////////////////////////////////////////////////

tipo_lista_param * formal_list1(tipo_param * param)
{
	tipo_lista_param * lista_param = inicializa_lista_param();
	lista_param = insere_param(lista_param, param);
	return lista_param;
}

int proc_header(char * nome_proc, tipo_lista_param * lista_param)
{
	static int end_valido = 0;
	
	tipo_simbolo * simb_proc = cria_simbolo(nome_proc, (void*)lista_param, end_valido, 5);
	instala_simbolo(tab_simbolos, simb_proc);
	
	insere_nivel(tab_simbolos);
	
	tipo_param * aux = lista_param->primeiro;
	int i = 0;
	while (aux != NULL)
	{
		tipo_simbolo * simb_param = cria_simbolo(aux->nome, NULL, (-(lista_param->num_param+4)+i), aux->tipo);
		instala_simbolo(tab_simbolos, simb_param);
		
		i+=1;
		aux = aux->prox;
	}
	
	return end_valido++;
}

tipo_lista_comandos * proc_decl(tipo_lista_param * lista_param, tipo_lista_comandos * comandos_proc)
{
	char nome_comando[TAM_COM];
	
	sprintf(nome_comando, "LIP%d ENTR 1", lista_param->endereco);
	tipo_comando * comando_entr = inicializa_comando(nome_comando);
	
	comando_entr->prox = comandos_proc->primeiro;
	comandos_proc->primeiro = comando_entr;
	
	
	sprintf(nome_comando, "DSVS LFP%d", lista_param->endereco);
	tipo_comando * comando_dsvs = inicializa_comando(nome_comando);
	
	comando_dsvs->prox = comandos_proc->primeiro;
	comandos_proc->primeiro = comando_dsvs;
	
	sprintf(nome_comando, "RTPR %d:", lista_param->num_param);
	insere_comando(comandos_proc, nome_comando);
	
	sprintf(nome_comando, "LFP%d:", lista_param->endereco);
	insere_comando(comandos_proc, nome_comando);
	

	remove_nivel(tab_simbolos);
	
	return comandos_proc;
}

//TODO: implementar chamada de procedimento e nível do procedimento no carregamento da variável (armazenar essa informação na tabela de símbolos)

