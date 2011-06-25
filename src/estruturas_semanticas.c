#include "estruturas_semanticas.h"
#include "floresta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern tipo_floresta * tab_simbolos;

void program(tipo_lista_comandos * lista_comandos)
{
	printf("INPP\n");
	
	printf("AMEM %d\n", lista_comandos->num_decl);
	
	tipo_comando * aux = lista_comandos->primeiro;
	while (aux != NULL)
	{
		if (aux->nome[strlen(aux->nome)-1] == ':'){
			aux->nome[strlen(aux->nome)-1] = '\0';
			printf("%s ", aux->nome);
		}
		else
			printf("%s\n", aux->nome);
		aux = aux->prox;
	}
	printf("PARA\n");
	
	libera_lista_comandos(lista_comandos);
	remove_tabela_simbolos(tab_simbolos);
}

tipo_lista_comandos * stmt_list(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);
	
	return lista_comandos;
}

int variable_decl(int tipo, tipo_lista_identificadores * lista_identificadores)
{
	static int valor_valido = 0;
	tipo_identificador * aux = lista_identificadores->primeiro;
	
	int num_ids = 0;
	while (aux != NULL)
	{	
		tipo_simbolo * aux_simb = cria_simbolo(aux->nome, NULL, valor_valido++, tipo);
		instala_simbolo(tab_simbolos, aux_simb);
		
		num_ids+=1;
		aux = aux->prox;
	}
	
	libera_lista_identificadores(lista_identificadores);
	
	return num_ids;
}

tipo_lista_identificadores * ident_list1(char * identificador)
{
	tipo_lista_identificadores * lista_identificadores = inicializa_lista_identificadores();
	
	insere_identificador(lista_identificadores, identificador);
	
	return lista_identificadores;
}

tipo_lista_identificadores * ident_list2(tipo_lista_identificadores * lista_identificadores, char * identificador)
{
	insere_identificador(lista_identificadores, identificador);
	
	return lista_identificadores;
}


char * identifier(char ident[])
{
	char * identificador = (char *)malloc(sizeof(char)*(TAM_IDENT+1));
	strcpy(identificador, ident);
	return identificador;
}

tipo_lista_comandos * read_stmt(tipo_lista_identificadores * lista_identificadores)
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	tipo_identificador * aux = lista_identificadores->primeiro;
	
	while (aux != NULL)
	{	
		tipo_simbolo * aux_simb = recupera_simbolo(tab_simbolos, aux->nome);
		
		char nome_comando[TAM_COM];
		sprintf(nome_comando, "LEIT");
		insere_comando(lista_comandos, nome_comando);
		
		sprintf(nome_comando, "ARMZ 0 %d", aux_simb->endereco);
		insere_comando(lista_comandos, nome_comando);
		
		aux = aux->prox;
	}
	
	return lista_comandos;
}

tipo_lista_comandos * factor1(char * identificador)
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	tipo_simbolo * aux_simb = recupera_simbolo(tab_simbolos, identificador);
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CRVL 0 %d", aux_simb->endereco);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * factor_a2(tipo_lista_comandos * lista_comandos)
{		
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "INVR");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * expr_bool_not2(tipo_lista_comandos * lista_comandos)
{		
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "NEGA");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * term2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "MULT");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * expr_bool_and2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CONJ");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * simple_expr2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "SOMA");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * expr_boolean2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "DISJ");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * assign_stmt(char * identificador, tipo_lista_comandos * lista_comandos)
{
	tipo_simbolo * simbolo = recupera_simbolo(tab_simbolos, identificador);
	
	int tipo_simbolo = simbolo->tipo;
	tipo_comando *ultimo_comando = lista_comandos->ultimo;
	if ((strncmp("CRCT",ultimo_comando->nome,4) ==  0) && tipo_simbolo == 1){ //erro real mas botou int
	  fprintf(stderr,"\nERRO! Atribuição inválida! Tipo declarado é diferente do tipo do valor da atribuição. Abortando...\n\n"); 
	  exit(0);
	}
	else
	  if ((strncmp("CRCF",ultimo_comando->nome,4) ==  0) && tipo_simbolo == 0){ //erro int mas botou real
	    fprintf(stderr,"\nERRO! Atribuição inválida! Tipo declarado é diferente do tipo do valor da atribuição. Abortando...\n\n");
	    exit(0);
	  }
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "ARMZ 0 %d", simbolo->endereco);
	insere_comando(lista_comandos, nome_comando);

	return lista_comandos;
}

tipo_lista_comandos * constant1(int const_int)
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CRCT %d", const_int);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * constant2(float const_real)
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CRCF %g", const_real);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}


tipo_lista_comandos * relop(int tipo_rel)
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	
	switch (tipo_rel)
	{
		case 0:
			sprintf(nome_comando, "CMIG");
			break;
		case 1:
			sprintf(nome_comando, "CMME");
			break;
		case 2:
			sprintf(nome_comando, "CMEG");
			break;
		case 3:
			sprintf(nome_comando, "CMDG");
			break;
	}
	
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * expression2(tipo_lista_comandos * lista_esq, tipo_lista_comandos * lista_rel, 
									tipo_lista_comandos * lista_dir)
{
	tipo_lista_comandos * lista_expressoes = concatena_listas_comandos(lista_esq, lista_dir);
	
	return concatena_listas_comandos(lista_expressoes, lista_rel);
}

tipo_lista_comandos * if_stmt1(tipo_lista_comandos * lista_condition, tipo_lista_comandos * lista_then)
{
	static int num_label_val = 0;
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "DSVF LI%d", num_label_val);
	insere_comando(lista_condition, nome_comando);
	
	sprintf(nome_comando, "LI%d:", num_label_val++);
	insere_comando(lista_then, nome_comando);
	
	return concatena_listas_comandos(lista_condition, lista_then);
}

tipo_lista_comandos * if_stmt2(tipo_lista_comandos * lista_condition, tipo_lista_comandos * lista_then,
								tipo_lista_comandos * lista_else)
{
	static int num_label_val = 0;
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "DSVF LIE%d", num_label_val);
	insere_comando(lista_condition, nome_comando);
	
	sprintf(nome_comando, "LIE%d %s", num_label_val++, lista_else->primeiro->nome);
	strcpy(lista_else->primeiro->nome, nome_comando);
	
	sprintf(nome_comando, "DSVS LIE%d", num_label_val);
	insere_comando(lista_then, nome_comando);
	
	sprintf(nome_comando, "LIE%d:", num_label_val++);
	insere_comando(lista_else, nome_comando);
	
	tipo_lista_comandos * lista1 = concatena_listas_comandos(lista_condition, lista_then);
	
	return concatena_listas_comandos(lista1, lista_else);
}

tipo_lista_comandos * expr_list1(tipo_lista_comandos * lista_expression)
{
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "BRANCO");
	insere_comando(lista_expression, nome_comando);
	
	return lista_expression;
}

tipo_lista_comandos * expr_list2(tipo_lista_comandos * expr_list, tipo_lista_comandos * expression)
{
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "BRANCO");
	insere_comando(expression, nome_comando);
	
	return concatena_listas_comandos(expr_list, expression);
}

tipo_lista_comandos * write_stmt(tipo_lista_comandos * lista_comandos)
{
	tipo_comando * aux = lista_comandos->primeiro;
	
	while (aux != NULL)
	{
		if (strcmp(aux->nome, "BRANCO") == 0)
		{
			char nome_comando[TAM_COM];
			sprintf(nome_comando, "IMPR");
			strcpy(aux->nome, nome_comando);
		}
		aux = aux->prox;
	}
	
	return lista_comandos;
}

tipo_lista_comandos * while_stmt(tipo_lista_comandos * conditions, tipo_lista_comandos * stmts)
{
	static int num_label_val = 0;
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "LW%d %s", num_label_val, conditions->primeiro->nome);
	strcpy(conditions->primeiro->nome, nome_comando);
	
	sprintf(nome_comando, "DSVS LW%d", num_label_val++);
	insere_comando(stmts, nome_comando);
	
	sprintf(nome_comando, "DSVF LW%d", num_label_val);
	insere_comando(conditions, nome_comando);
	
	sprintf(nome_comando, "LW%d:", num_label_val);
	insere_comando(stmts, nome_comando);
	
	return concatena_listas_comandos(conditions, stmts);
}

///////////////////////////////////////////////////////////////////////////////

void libera_lista_comandos(tipo_lista_comandos * lista_comandos)
{
	while (lista_comandos->primeiro != NULL)
	{
		tipo_comando * aux = lista_comandos->primeiro;
		lista_comandos->primeiro = aux->prox;
		free(aux);
	}
}

tipo_lista_comandos * inicializa_lista_comandos()
{
	tipo_lista_comandos * lista_comandos = (tipo_lista_comandos *)malloc(sizeof(tipo_lista_comandos));
	lista_comandos->primeiro = NULL;
	lista_comandos->ultimo = NULL;
	lista_comandos->num_decl = 0;
	
	return lista_comandos;
}

tipo_lista_comandos * concatena_listas_comandos(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	tipo_comando * aux_comando = lista2->primeiro;
	
	while (aux_comando != NULL)
	{
		lista1->ultimo->prox = aux_comando;
		lista1->ultimo = aux_comando;
		
		aux_comando = aux_comando->prox;
	}
	
	free(lista2);
	
	return lista1;
}

tipo_comando * inicializa_comando(char nome_comando[])
{
	tipo_comando * comando = (tipo_comando*)malloc(sizeof(tipo_comando));
	strcpy(comando->nome, nome_comando);
	comando->prox = NULL;
	
	return comando;
}

void insere_comando(tipo_lista_comandos * lista_comandos, char nome_comando[])
{
	tipo_comando * comando = inicializa_comando(nome_comando);
	
	if (lista_comandos->ultimo == NULL)
	{
		lista_comandos->primeiro = comando;
	}
	else
	{
		lista_comandos->ultimo->prox = comando;
	}
		
	
	lista_comandos->ultimo = comando;
}

tipo_lista_identificadores * inicializa_lista_identificadores()
{
	tipo_lista_identificadores * lista_identificadores = (tipo_lista_identificadores *)malloc(sizeof(tipo_lista_identificadores));
	lista_identificadores->primeiro = NULL;
	lista_identificadores->ultimo = NULL;
	
	return lista_identificadores;
}

void insere_identificador(tipo_lista_identificadores * lista_identificadores, char * nome_identificador)
{
	tipo_identificador * identificador = (tipo_identificador *)malloc(sizeof(tipo_identificador));
	strcpy(identificador->nome, nome_identificador);
	identificador->prox = NULL;
	
	if (lista_identificadores->ultimo == NULL)
	{
		lista_identificadores->primeiro = identificador;
	}
	else
	{
		lista_identificadores->ultimo->prox = identificador;
	}
		
	
	lista_identificadores->ultimo = identificador;
}

void libera_lista_identificadores(tipo_lista_identificadores * lista_identificadores)
{
	while (lista_identificadores->primeiro != NULL)
	{
		tipo_identificador * aux = lista_identificadores->primeiro;
		lista_identificadores->primeiro = aux->prox;
		free(aux);
	}
}
