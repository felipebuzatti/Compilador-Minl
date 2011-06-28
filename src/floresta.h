#ifndef FLORESTA_H_INCLUDED
#define FLORESTA_H_INCLUDED

#define MAX_NOME_SIMBOLO 100

#include "procedimentos.h"



typedef struct{
	char lexema[MAX_NOME_SIMBOLO];
	int endereco;
	int tipo; //INTEGER = 0; REAL=1; BOOLEAN=2; CHAR=3; LABEL=4; PROCEDURE=5
	int num_param;
} tipo_simbolo;

typedef struct tipo_no_arvore_str{
	tipo_simbolo * simbolo;
	struct tipo_no_arvore_str * esq;
	struct tipo_no_arvore_str * dir;
} tipo_no_arvore;

typedef tipo_no_arvore tipo_arvore;

/*
    Tipo_cel compõe uma lista encadeada em que cada célula contém um apontador
  para um árvore, constituindo, em verdade, essa lista encadeada, uma floresta,
  que nada mais é que nossa tabela de símbolos.
*/
typedef struct tipo_cel_str{
	tipo_arvore * arvore;
	struct tipo_cel_str * ant;
	struct tipo_cel_str * prox;
} tipo_cel;

typedef struct{
	tipo_cel * primeiro;
	tipo_cel * ultimo;
} tipo_floresta;

void inicializa_tabela_simbolos(tipo_floresta * floresta);
void insere_nivel(tipo_floresta * floresta);
void remove_nivel(tipo_floresta * floresta);
tipo_simbolo * cria_simbolo(char lexema[MAX_NOME_SIMBOLO], int num_param, int endereco, int tipo);
void libera_simbolo(tipo_simbolo * simbolo);
void instala_simbolo(tipo_floresta * floresta, tipo_simbolo * simbolo);
tipo_simbolo * recupera_simbolo(tipo_floresta * floresta, char * lexema);
void remove_tabela_simbolos(tipo_floresta * floresta);
void libera_arvore(tipo_arvore * arvore);


#endif /* FLORESTA_H_INCLUDED */
