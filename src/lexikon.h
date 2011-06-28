#include <stdio.h>

#define TAM_BUFFER_ENTR 300
#define TAM_BUFFER_ESP 3
#define TAM_TIPO_SIMB 50

typedef short flag;

typedef struct item_lex_str{
	int token;
	void * value;
	struct item_lex_str * prox;
} item_lex;

/*
	Faz a análise léxica de uma linha do arquivo. Retorna 1 quando encontra
uma quebra de linha, e 0 quando encontra final do arquivo.
*/
int analise_lex(FILE * arquivo, item_lex * tokens);
item_lex * exporta_buffer(char buffer[TAM_BUFFER_ENTR], int * ind_buffer, flag tipo);
item_lex * exporta_simbolo(char valor);
item_lex * exporta_char(char valor);
item_lex * exporta_buffer_esp(char valor[TAM_BUFFER_ESP], int cod_token);

//========HASH palavras reservadas=======//

typedef struct{
	char valor[TAM_BUFFER_ENTR];
	int cod_token;
} tipo_entr_hash;

#define TAM_HASH 50

//retorna o hash, já contendo as palavras reservadas
tipo_entr_hash ** inicializa_hash();
void insere_hash(tipo_entr_hash ** tab_hash, char * valor, int cod_token);
int calcula_hash(char * termo);
//retorna 1 caso a palavra passada seja reservada, 0 caso contrário
int consulta_hash(tipo_entr_hash ** tab_hash, char palavra[TAM_BUFFER_ENTR], int * cod_token);
void libera_hash(tipo_entr_hash ** tab_hash);

