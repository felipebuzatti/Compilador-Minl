#include <stdio.h>
#include "lexikon.h"
#include "estruturas_semanticas.h"
#include "floresta.h"
#include <stdlib.h>

//declara tabela hash
tipo_entr_hash ** tabela;

tipo_floresta * tab_simbolos;

int nivel_processo = 0;

int main (int argc, char *argv[]){
	if (argc > 1){
		freopen(argv[1], "r", stdin);
	}
	if (argc > 2)
	{
		freopen(argv[2], "w", stdout);
	}
	//inicializa tab_hash, que é uma variável global
	tabela = inicializa_hash();
	
	tab_simbolos = (tipo_floresta*)malloc(sizeof(tipo_floresta));
	inicializa_tabela_simbolos(tab_simbolos);
	
	yyparse();
	
	
	libera_hash(tabela);
	return 0;
}
