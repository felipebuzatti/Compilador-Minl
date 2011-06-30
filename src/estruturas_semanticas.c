#include "estruturas_semanticas.h"
#include "floresta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern tipo_floresta * tab_simbolos;

extern int nivel_processo;

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

tipo_lista_comandos * decl1(int num_decl)
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	lista_comandos->num_decl = num_decl;
	
	return lista_comandos;
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
		tipo_simbolo *verifica = recupera_simbolo(tab_simbolos, aux->nome);
		if (verifica == NULL){ 
		  tipo_simbolo * aux_simb = cria_simbolo(aux->nome, 0, valor_valido++, tipo);
		  instala_simbolo(tab_simbolos, aux_simb);		
		  num_ids+=1;
		}
		else{
		  fprintf(stderr,"ERRO! Redefinição de tipo da variável '%s'! Abortando...\n\n", aux->nome);
		  exit(0);
		}
		
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
		
		sprintf(nome_comando, "ARMZ %d %d", nivel_processo, aux_simb->endereco);
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
	sprintf(nome_comando, "CRVL %d %d", nivel_processo, aux_simb->endereco);
	insere_comando(lista_comandos, nome_comando);
	seta_tipo_lista(lista_comandos, aux_simb->tipo);
	
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
	if(lista_comandos->tipo != 2) {
	  fprintf(stderr,"ERRO! Negação de tipo não booleano! Abortando...\n\n");
	  exit(0);
	}
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "NEGA");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * term2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	if (verifica_tipos(lista1, lista2,0) == 0){//tipo = 0: não pode ter bol
	  fprintf(stderr,"\nERRO! Multiplicação incompatível (tipos diferentes)! Abortando...\n\n"); 
	  exit(0);
	}
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "MULT");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * expr_bool_and2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	if (verifica_tipos(lista1, lista2, 1) == 0){//tipo =1: só pode ter bol
	    fprintf(stderr,"\nERRO! Esperado(s) booleano(s) para a conjunção (AND)! Abortando...\n\n"); 
	    exit(0);
	}
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CONJ");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * simple_expr2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
 	if (verifica_tipos(lista1, lista2, 0) == 0){//tipo = 0: não pode ter bol
	  fprintf(stderr,"\nERRO! Soma incompatível (tipos diferentes)! Abortando...\n\n"); 
	  exit(0);
	}
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "SOMA");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * expr_boolean2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	if (verifica_tipos(lista1, lista2, 1) == 0){//tipo =1: só pode ter bol
	    fprintf(stderr,"\nERRO! Esperado(s) booleano(s) para a disjunção (OR)! Abortando...\n\n"); 
	    exit(0);
	}
	tipo_lista_comandos * lista_comandos = concatena_listas_comandos(lista1, lista2);

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "DISJ");
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * assign_stmt(char * identificador, tipo_lista_comandos * lista_comandos)
{
	tipo_simbolo * simbolo = recupera_simbolo(tab_simbolos, identificador);
	
	if (simbolo == NULL){
		fprintf(stderr, "\nERRO! Variável '%s' não foi declarada! Abortando...\n\n", identificador);
		exit(0);
	}
	
	int tipo_simbolo = simbolo->tipo;
	if (tipo_simbolo != lista_comandos->tipo)
	{
		fprintf(stderr, "\nERRO! Atribuição inválida na variável '%s'! Abortando...\n\n", identificador);
		exit(0);
	}

	char nome_comando[TAM_COM];
	sprintf(nome_comando, "ARMZ %d %d", nivel_processo, simbolo->endereco);
	insere_comando(lista_comandos, nome_comando);

	return lista_comandos;
}

tipo_lista_comandos * constant1(int const_int) //int
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CRCT %d", const_int);
	seta_tipo_lista(lista_comandos, 0);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * constant2(float const_real)//real
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CRCF %f", const_real);
	seta_tipo_lista(lista_comandos, 1);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * constant3(int const_int)//bool
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CRCT %d", const_int);
	seta_tipo_lista(lista_comandos, 2);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

tipo_lista_comandos * constant4(char const_int)//char
{
	tipo_lista_comandos * lista_comandos = inicializa_lista_comandos();
	
	char nome_comando[TAM_COM];
	sprintf(nome_comando, "CRCT %d", const_int);
	seta_tipo_lista(lista_comandos, 3);
	insere_comando(lista_comandos, nome_comando);
	
	return lista_comandos;
}

int relop(int tipo_rel)
{
	return tipo_rel;
}

tipo_lista_comandos * expression2(tipo_lista_comandos * lista_esq, int tipo_rel, tipo_lista_comandos * lista_dir)
{
	if (verifica_tipos(lista_esq, lista_dir,-1) == 0){//sem parâmetros
	  fprintf(stderr,"\nERRO! Operações relacionais de tipos diferentes! Abortando...\n\n"); 
	  exit(0);
	}
	tipo_lista_comandos * lista_expressoes = concatena_listas_comandos(lista_esq, lista_dir);
		
	char nome_comando[TAM_COM];
	
	// tipo_rel possui tipo 0 para EQ, 1 para LT, 2 para LE e 3 para DIF
	switch (tipo_rel)
	{
		case 0:
			if (lista_expressoes->tipo == 0)
			  sprintf(nome_comando, "CMIG"); //i1=i2
			if (lista_expressoes->tipo == 1)
			  sprintf(nome_comando, "CMIF"); //f1=f2
			break;
		case 1:
			if (lista_expressoes->tipo == 0)    
			  sprintf(nome_comando, "CMME"); //i1<i2
			if (lista_expressoes->tipo == 1)
			  sprintf(nome_comando, "CMNF"); //f1<f2
			break;
		case 2:
			if (lista_expressoes->tipo == 0)			
			  sprintf(nome_comando, "CMEG"); //i1<=i2
			if (lista_expressoes->tipo == 1)
			  sprintf(nome_comando, "CMEF"); //f1<=f2
			break;
		case 3:
			if (lista_expressoes->tipo == 0)
			  sprintf(nome_comando, "CMDG"); //i1!=i2
			if (lista_expressoes->tipo == 1)			
			  sprintf(nome_comando, "CMDF"); //f1!=f2
			break;
	}
		
	
	insere_comando(lista_expressoes, nome_comando);
	seta_tipo_lista(lista_expressoes, 2);
	return lista_expressoes;
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

tipo_lista_lista_comandos * expr_list1(tipo_lista_comandos * lista_expression)
{
	tipo_lista_lista_comandos * lista_lista = inicializa_lista_lista_comandos();
	insere_lista_comandos(lista_lista, lista_expression);
	return lista_lista;
}

tipo_lista_lista_comandos * expr_list2(tipo_lista_lista_comandos * expr_list, tipo_lista_comandos * expression)
{
	insere_lista_comandos(expr_list, expression);
	return expr_list;
}

tipo_lista_comandos * write_stmt(tipo_lista_lista_comandos * lista_lista_comandos)
{
	tipo_lista_comandos * aux = lista_lista_comandos->primeiro;
	tipo_lista_comandos * lista_retorno = NULL;
	
	char nome_comando[TAM_COM];
	
	while (aux != NULL)
	{
		switch(aux->tipo)
		{
			case 0: //int
			  sprintf(nome_comando, "IMPR"); 
			  break;
			case 1: //real
			  sprintf(nome_comando, "IMPF"); 
			  break;
			case 2: //bool
			  sprintf(nome_comando, "IMPR"); 
			  break;
			case 3: //char
			  sprintf(nome_comando, "IMPC"); 
			  break;			
		}
		insere_comando(aux, nome_comando);
	
		if (lista_retorno == NULL)
		{
			lista_retorno = aux;
		}
		else
		{
			lista_retorno = concatena_listas_comandos(lista_retorno, aux);
		}
		
		aux = aux->prox;
	}
	
	return lista_retorno;
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
	lista_comandos->prox = NULL;
	
	return lista_comandos;
}

tipo_lista_comandos * concatena_listas_comandos(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2)
{
	
	if (lista1->ultimo == NULL)
	{
		lista2->num_decl +=  lista1->num_decl;
		free(lista1);
		return lista2;
	}

	tipo_comando * aux_comando = lista2->primeiro;
	
	while (aux_comando != NULL)
	{
		lista1->ultimo->prox = aux_comando;
		lista1->ultimo = aux_comando;
		
		aux_comando = aux_comando->prox;
	}
	
	lista1->num_decl += lista2->num_decl;
	
	free(lista2);
	
	return lista1;
}

tipo_lista_lista_comandos * inicializa_lista_lista_comandos()
{
	tipo_lista_lista_comandos * lista_lista_comandos = (tipo_lista_lista_comandos *)malloc(sizeof(tipo_lista_lista_comandos));
	lista_lista_comandos->primeiro = NULL;
	lista_lista_comandos->ultimo = NULL;
	
	return lista_lista_comandos;
}

void libera_lista_lista_comandos(tipo_lista_lista_comandos * lista_lista_comandos)
{
	while (lista_lista_comandos->primeiro != NULL)
	{
		tipo_lista_comandos * aux = lista_lista_comandos->primeiro;
		lista_lista_comandos->primeiro = aux->prox;
		free(aux);
	}
}

tipo_lista_lista_comandos * concatena_listas_lista_comandos(tipo_lista_lista_comandos * lista1, tipo_lista_lista_comandos * lista2)
{
	
	if (lista1->ultimo == NULL)
	{
		free(lista1);
		return lista2;
	}

	tipo_lista_comandos * aux_lista = lista2->primeiro;
	
	while (aux_lista != NULL)
	{
		lista1->ultimo->prox = aux_lista;
		lista1->ultimo = aux_lista;
		
		aux_lista = aux_lista->prox;
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

void insere_lista_comandos(tipo_lista_lista_comandos * lista_lista_comandos, tipo_lista_comandos * lista_comandos)
{
	
	if (lista_lista_comandos->ultimo == NULL)
	{
		lista_lista_comandos->primeiro = lista_comandos;
	}
	else
	{
		lista_lista_comandos->ultimo->prox = lista_comandos;
	}
		
	
	lista_lista_comandos->ultimo = lista_comandos;
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

void seta_tipo_lista(tipo_lista_comandos * lista, int tipo)
{
    lista->tipo = tipo;
    return;
}
 
int verifica_tipos(tipo_lista_comandos * lista_esq, tipo_lista_comandos * lista_dir, int tipo)
{
    //TIPO = 0: não pode ter booleanos
    //     = 1: só pode ter booleanos
    //     =-1: sem restrições
    if (tipo == -1){
	if (lista_esq->tipo == lista_dir->tipo)
	  return 1;
	else{
	  if (lista_esq->tipo == -1 || lista_dir->tipo == -1)
	    return 1;
	  else
	    return 0;
	}
    }
    if (tipo == 1){
      if (lista_esq->tipo != 2 && lista_dir->tipo != 2)
	  return 0;
      else return 1;
    }
    if(tipo == 0){
      if (lista_esq->tipo == 2 && lista_dir->tipo == 2)
	  return 0;
      if (lista_esq->tipo == lista_dir->tipo)
	  return 1;
	else{
	  if (lista_esq->tipo == -1 || lista_dir->tipo == -1)
	    return 1;
	  else
	    return 0;
	}
    }
    return 1;

}
