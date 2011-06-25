#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexikon.h"
#include "estruturas_semanticas.h"
#include "parser.tab.h"

//macros para trabalhar com a flag tipo. Testam se o tipo se refere a uma 
//palavra, um número, e de que tipo.
#define EH_NUMERO ((tipo & 1) == 1)
#define EH_PALAVRA ((tipo & 2) == 2)
#define EH_INTEIRO ((tipo & 4) == 4)
#define EH_REAL ((tipo & 8) == 8)

extern tipo_entr_hash ** tabela;

int analise_lex(FILE * arquivo, item_lex * tokens){
	char c = fgetc(arquivo);
	char buffer[TAM_BUFFER_ENTR];
	char buffer_esp[3];
	
	/*
		flag que guarda informações sobre o buffer a ser exportado. 
	Utiliza-se os bits da flag para guardar as informações. Dessa forma o
	bit zero define se o buffer contém um número, o bit um, uma palavra, o 
	dois, se é um número inteiro, e o bit quatro se é um númeroreal.
		Isso significa que um número real teria essa flag com o valor
	9. que é 00001001; e uma palavra teria tipo==2, ou seja 00000010.
	*/
	flag tipo=0;
	
	int ind_buffer=0; //marca ponto em que o caractere deve ser inserido no 
									//buffer
	
	while(!feof(arquivo)){
		switch(c){
			case '+':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				break;
			case '*':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				break;
			case '\'':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				c = fgetc(arquivo);
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				c = fgetc(arquivo);
				break;
			case '(':
			case ')':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				break;
			case '[':
			case ']':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				break;
			case ',':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				break;
			case ';':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				break;
			case '=':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
					
				buffer_esp[1] = '\0';
				tokens->prox = exporta_buffer_esp(buffer_esp, EQ);
				tokens = tokens->prox;
				break;
			case '<':
			case '!':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				buffer_esp[0] = c;
				if ((c = fgetc(arquivo)) == '='){
					buffer_esp[1] = c;
					buffer_esp[2] = '\0';
					tokens->prox = exporta_buffer_esp(buffer_esp, DIF);
					tokens = tokens->prox;
				} else{
					fseek(arquivo, -1, SEEK_CUR); //retorna
								//caractere lido
					buffer_esp[1] = '\0';
					tokens->prox = exporta_buffer_esp(buffer_esp, LT);
					tokens = tokens->prox;
				}
				break;
			case ':':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				buffer_esp[0] = c;
				if ((c = fgetc(arquivo)) == '='){
					buffer_esp[1] = c;
					buffer_esp[2] = '\0';
					tokens->prox = exporta_buffer_esp(buffer_esp, ATRIBUI);
					tokens = tokens->prox;
				} else{
					fseek(arquivo, -1, SEEK_CUR); //retorna
								//caractere lido
					tokens->prox = exporta_simbolo(buffer_esp[0]);
					tokens = tokens->prox;
				}
				break;
			case '\n':
				tokens->prox = exporta_buffer(buffer,&ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				return 1;
			case ' ': //espaços representam final de um termo, ou são ignorados
			case '\t':
				tokens->prox = exporta_buffer(buffer,&ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				
				//reinicia o buffer
				ind_buffer=0;
				break;
			case 'E':
			case '.':
				buffer[ind_buffer++] = c;
				if (EH_NUMERO){
					tipo = 9; //trata-se de um número real
				}
				break;
			case '-':
				tokens->prox = exporta_buffer(buffer, &ind_buffer, tipo);
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				tokens->prox = exporta_simbolo(c);
				tokens = tokens->prox;
				if (tokens->prox != NULL)
					tokens = tokens->prox;
				break;
				
			default: //caso em que pode se tratar de uma letra ou número
				if (ind_buffer == 0) {
					if (isdigit(c)){
						tipo = 5; //define que se trata de um número, inteiro, até que se prove o contrário
					} else if (isalpha(c) || c == '_'){
						tipo=1; //define que se trata de uma palavra
					}
				}
				if (isalnum(c) || (!EH_NUMERO  && c == '_')){
					buffer[ind_buffer++] = c; //lê para buffer
				} else{
					fprintf(stderr, "\nErro: caractere %c não reconhecido\n", c);
					exit(1);
				}
				break;
				
		}
		c = fgetc(arquivo);
		
		
		
	}
	
	return 0;
}

item_lex * exporta_buffer(char buffer[TAM_BUFFER_ENTR], int * ind_buffer, flag tipo){
	int cod_token;
	
	if ((*ind_buffer) == 0){
		return NULL; //significa que o buffer ainda não tem nenhum caractere
	}
	
	buffer[(*ind_buffer)++] = '\0';
	item_lex * aux_item = (item_lex*)malloc(sizeof(item_lex));
	aux_item->prox = NULL;
	if (EH_INTEIRO){
		aux_item->token = UNSIG_INT;
		aux_item->value = (int*)malloc(sizeof(int));
		int value_int = atoi(buffer);
		memcpy(aux_item->value, &value_int, sizeof(int));
	} else if (EH_REAL){
		aux_item->token = UNSIG_REAL;
		aux_item->value = (float*)malloc(sizeof(float));
		float value_real = atof(buffer);
		memcpy(aux_item->value, &value_real, sizeof(float));
		
	} else if (consulta_hash(tabela, buffer, &cod_token)){ //se é palavra reservada
		aux_item->token = cod_token;
		aux_item->value = (int*)malloc(sizeof(char)*strlen(buffer)+1);
		strcpy(aux_item->value, buffer);
	} else{
		aux_item->token = IDENT;
		aux_item->value = (char *)malloc(sizeof(char)*strlen(buffer)+1);
		strcpy(aux_item->value, buffer);
	}
	
	(*ind_buffer) = 0;
	return aux_item;
}

item_lex * exporta_simbolo(char valor){
	item_lex * aux_item = (item_lex*)malloc(sizeof(item_lex));
	aux_item->token = valor;
	aux_item->value = NULL;
	aux_item->prox = NULL;
	return aux_item;
}

item_lex * exporta_buffer_esp(char valor[TAM_BUFFER_ESP], int cod_token){
	item_lex * aux_item = (item_lex*)malloc(sizeof(item_lex));
	aux_item->token = cod_token;
	aux_item->value = (char *)malloc(sizeof(char)*strlen(valor)+1);
	strcpy(aux_item->value, valor);
	aux_item->prox = NULL;
	return aux_item;
}

//========HASH palavras reservadas=======//

tipo_entr_hash ** inicializa_hash(){
	tipo_entr_hash ** tab_hash;
	tab_hash = malloc(sizeof(tipo_entr_hash *)*TAM_HASH);
	
	int i;
	for (i = 0; i < TAM_HASH; i += 1){
		tab_hash[i] = NULL; //zera as posições da tabela.
	}
	
	//Insere as palavras reservadas (O número de entradas aqui não pode ultrapassar TAM_HASH):
	insere_hash(tab_hash, "program", PROGRAM);
	insere_hash(tab_hash, "declare", DECLARE);
	insere_hash(tab_hash, "do", DO);
	insere_hash(tab_hash, "integer", INTEGER);
	insere_hash(tab_hash, "real", REAL);
	insere_hash(tab_hash, "boolean", BOOLEAN);
	insere_hash(tab_hash, "char", CHAR);
	insere_hash(tab_hash, "label", LABEL);
	insere_hash(tab_hash, "array", ARRAY);
	insere_hash(tab_hash, "of", OF);
	insere_hash(tab_hash, "procedure", PROCEDURE);
	insere_hash(tab_hash, "if", IF);
	insere_hash(tab_hash, "then", THEN);
	insere_hash(tab_hash, "else", ELSE);
	insere_hash(tab_hash, "end", END);
	insere_hash(tab_hash, "while", WHILE);
	insere_hash(tab_hash, "do", DO);
	insere_hash(tab_hash, "read", READ);
	insere_hash(tab_hash, "write", WRITE);
	insere_hash(tab_hash, "goto", GOTO);
	insere_hash(tab_hash, "return", RETURN);
	insere_hash(tab_hash, "not", NOT);
	insere_hash(tab_hash, "or", OR);
	insere_hash(tab_hash, "and", AND);
	insere_hash(tab_hash, "false", BOOL_CONST_F);
	insere_hash(tab_hash, "true", BOOL_CONST_T);
	
	//retorna o hash criado
	return tab_hash;
}

void insere_hash(tipo_entr_hash ** tab_hash, char * valor, int cod_token){
	//procura por uma posição para inserir
	int posicao_hash = calcula_hash(valor);;
	while (tab_hash[posicao_hash] != NULL){ //procura uma posição livre da tabela
		posicao_hash+=1;
	}
	
	tipo_entr_hash * nova = malloc(sizeof(tipo_entr_hash));
	strcpy(nova->valor, valor);
	nova->cod_token = cod_token;
	
	tab_hash[posicao_hash] = nova;
}

int calcula_hash(char * termo){
	int resultado=0;
	
	int i;
	for (i = 0; termo[i] != '\0'; i += 1){
		resultado = (resultado+(int)termo[i])%TAM_HASH;
	}
	
	return resultado;
}

int consulta_hash(tipo_entr_hash ** tab_hash, char palavra[TAM_BUFFER_ENTR], int * cod_token){
	int valor_hash = calcula_hash(palavra);
	
	while (tab_hash[valor_hash] != NULL && strcmp(tab_hash[valor_hash]->valor, palavra) != 0){
		valor_hash+=1;
	}
	
	if (tab_hash[valor_hash] == NULL){
			return 0;
	} else{
		(*cod_token) = tab_hash[valor_hash]->cod_token;
		return 1;
	}
}

void libera_hash(tipo_entr_hash ** tab_hash){
	int i;
	for (i = 0; i < TAM_HASH; i += 1){
		if (tab_hash[i] != NULL){
			free(tab_hash[i]);
		}
	}
	
	free(tab_hash);
}
