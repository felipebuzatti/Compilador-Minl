#include "procedimentos.h"
#include "estruturas_semanticas.h"
#include "floresta.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern tipo_floresta * tab_simbolos;

extern int nivel_processo;

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
	
	tipo_simbolo * simb_proc = cria_simbolo(nome_proc, lista_param->num_param, end_valido, 5);
	instala_simbolo(tab_simbolos, simb_proc);
	
	insere_nivel(tab_simbolos);
	nivel_processo+=1;
	
	tipo_param * aux = lista_param->primeiro;
	int i = 0;
	while (aux != NULL)
	{
		tipo_simbolo * simb_param = cria_simbolo(aux->nome, 0, (-(lista_param->num_param+4)+i), aux->tipo);
		instala_simbolo(tab_simbolos, simb_param);
		
		i+=1;
		aux = aux->prox;
	}
	
	return end_valido++;
}

tipo_lista_comandos * proc_stmt1(char * identificador)
{
	tipo_simbolo * simbolo_processo = recupera_simbolo(tab_simbolos, identificador);
	if (simbolo_processo->num_param != 0)
	{
		fprintf(stderr, "\nERRO! Processo %s chamado sem parâmetros\n\n", identificador);
		exit(0);
	}
	
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	
	sprintf(nome_comando, "CHPR LIP%d %d", simbolo_processo->endereco, nivel_processo);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * proc_stmt2(char * identificador, tipo_lista_comandos * lista_comandos)
{
	tipo_simbolo * simbolo_processo = recupera_simbolo(tab_simbolos, identificador);
	
	int num_param_chamados = 0;
	tipo_comando * aux = lista_comandos->primeiro;
	
	while (aux->prox != NULL)
	{
		if (strcmp(aux->prox->nome, "BRANCO") == 0)
		{
			num_param_chamados+=1;
			if (aux->prox == lista_comandos->ultimo)
			{
				lista_comandos->ultimo = aux;
				free(aux->prox);
				aux->prox = NULL;
			}
			else{
				if (aux->prox == lista_comandos->primeiro)
					lista_comandos->primeiro = aux->prox;
				
				tipo_comando * aux_libera = aux->prox;
				aux->prox = aux->prox->prox;
				free(aux_libera);
			}
		}
		else
			aux = aux->prox;
	}
	
	if (num_param_chamados != simbolo_processo->num_param)
	{
		fprintf(stderr, "\nNúmero de parâmetros na chamada do processo %s incompatível\n\n", identificador);
		exit(0);
	}
	
	char nome_comando[TAM_COM];
	
	sprintf(nome_comando, "CHPR LIP%d %d", simbolo_processo->endereco, nivel_processo);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
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
	
	sprintf(nome_comando, "RTPR %d", lista_param->num_param);
	insere_comando(comandos_proc, nome_comando);
	
	sprintf(nome_comando, "LFP%d:", lista_param->endereco);
	insere_comando(comandos_proc, nome_comando);
	

	remove_nivel(tab_simbolos);
	nivel_processo-=1;
	
	return comandos_proc;
}

//TODO: implementar chamada de procedimento e nível do procedimento no carregamento da variável (armazenar essa informação na tabela de símbolos)

