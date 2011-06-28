#include "floresta.h"
#include <stdlib.h> //malloc, exit
#include <string.h> //strcmp
#include <stdio.h> //fprintf
#include "procedimentos.h"

void inicializa_tabela_simbolos(tipo_floresta * floresta){
	floresta->primeiro = NULL;
	floresta->ultimo = NULL;
	
	insere_nivel(floresta);
}

void insere_nivel(tipo_floresta * floresta){
	tipo_cel * cel_floresta = malloc(sizeof(tipo_cel));
	cel_floresta->arvore = NULL;
	cel_floresta->ant = NULL;
	cel_floresta->prox = NULL;
	
	if (floresta->primeiro == NULL){
		floresta->primeiro = cel_floresta;
	}
	
	if (floresta->ultimo == NULL){
		floresta->ultimo = cel_floresta;
	} else{
		floresta->ultimo->prox = cel_floresta;
		cel_floresta->ant = floresta->ultimo;
		floresta->ultimo = cel_floresta;
	}
}

void remove_nivel(tipo_floresta * floresta){
	if (floresta->primeiro == NULL || floresta->ultimo == NULL){
		fprintf(stderr, "\nFloresta vazia, não é possível remover nível\n");
		exit(1);
	}
	
	tipo_cel * aux = floresta->ultimo;
	floresta->ultimo = aux->ant;
	
	if (floresta->ultimo == NULL){
		floresta->primeiro = NULL;
	} else{
		floresta->ultimo->prox = NULL;
	}
	
	if (aux->arvore != NULL){
		libera_arvore(aux->arvore);
	}
	
	free(aux);
}

void libera_arvore(tipo_arvore * arvore){
	if (arvore->esq != NULL){
		libera_arvore(arvore->esq);
	}
	if (arvore->dir != NULL){
		libera_arvore(arvore->dir);
	}
	libera_simbolo(arvore->simbolo);
	free(arvore);
}

tipo_simbolo * cria_simbolo(char lexema[MAX_NOME_SIMBOLO], int num_param, int endereco, int tipo){
	tipo_simbolo * aux = malloc(sizeof(tipo_simbolo));
	strcpy(aux->lexema, lexema);
	aux->num_param = num_param;
	aux->endereco = endereco;
	aux->tipo = tipo;
	
	return aux;
}

void libera_simbolo(tipo_simbolo * simbolo){
	free(simbolo);
}

tipo_no_arvore * aux_instala_simbolo(tipo_no_arvore * arvore, 
							tipo_simbolo * simbolo){
	if (arvore == NULL){
		tipo_no_arvore * aux = malloc(sizeof(tipo_no_arvore));
		aux->simbolo = simbolo;
		aux->esq = NULL;
		aux->dir = NULL;
		
		return aux;
	} else if (strcmp(simbolo->lexema, arvore->simbolo->lexema) < 0){
		arvore->esq = aux_instala_simbolo(arvore->esq, simbolo);
		return arvore;
	} else if (strcmp(simbolo->lexema, arvore->simbolo->lexema) > 0){
		arvore->dir = aux_instala_simbolo(arvore->dir, simbolo);
		return arvore;
	} else{ //simbolo->lexema == arvore->simbolo->lexema
		if (simbolo != arvore->simbolo) //garante que o apontador não é 
								//o mesmo
			libera_simbolo(simbolo); //libera, pois árvore já o 
									//contém
		
		return arvore;
	}
}

void instala_simbolo(tipo_floresta * floresta, tipo_simbolo * simbolo){
	//insere na árvore do último nível
	floresta->ultimo->arvore = 
			aux_instala_simbolo(floresta->ultimo->arvore, simbolo);
}

tipo_simbolo * aux_recupera_simbolo(tipo_arvore * arvore, char * lexema){
	if (arvore == NULL){
		return NULL;
	} else if (strcmp(lexema, arvore->simbolo->lexema) < 0){
		return aux_recupera_simbolo(arvore->esq, lexema);
	} else if (strcmp(lexema, arvore->simbolo->lexema) == 0){
		return arvore->simbolo;
	} else{
		return aux_recupera_simbolo(arvore->dir, lexema);
	}
}

tipo_simbolo * recupera_simbolo(tipo_floresta * floresta, char * lexema){
	tipo_simbolo * aux_simbolo = NULL;
	tipo_cel * aux_cel;
	
	aux_cel = floresta->ultimo;
	while (aux_cel != NULL){ //pesquisa em cada árvore da lista no sentido
				//da última célula para a primeira
		if (aux_cel->arvore != NULL)
			aux_simbolo = aux_recupera_simbolo(aux_cel->arvore, 
									lexema);
		if (aux_simbolo != NULL)
			return aux_simbolo;

		aux_cel = aux_cel->ant;
	}
	
	return NULL;
}

void remove_tabela_simbolos(tipo_floresta * floresta){
	if (floresta->ultimo != NULL){
		tipo_cel * aux = floresta->ultimo;
		floresta->ultimo = aux->ant;
		if (aux->arvore != NULL){
			libera_arvore(aux->arvore);
		}
		aux->ant = NULL;
		aux->prox = NULL;
		
		free(aux);
		
		remove_tabela_simbolos(floresta);
	} else{
		floresta->primeiro = NULL;
		free(floresta);
	}
}

