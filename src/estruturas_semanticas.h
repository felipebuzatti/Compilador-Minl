#ifndef ESTRUTURAS_SEMANTICAS_H_INCLUDED
#define ESTRUTURAS_SEMANTICAS_H_INCLUDED

#define TAM_COM 128
#define TAM_IDENT 255


typedef struct str_comando
{
	char nome[TAM_COM+1];
	struct str_comando * prox;
} tipo_comando;

typedef struct str_lista_comandos
{
	tipo_comando * primeiro;
	tipo_comando * ultimo;
	int num_decl;
	int tipo; //0 para int, 1 para real, 2 para bol, 3 para char
	struct str_lista_comandos * prox;
} tipo_lista_comandos;

typedef struct 
{
	tipo_lista_comandos * primeiro;
	tipo_lista_comandos * ultimo;
} tipo_lista_lista_comandos;

typedef struct str_identificador
{
	char nome[TAM_IDENT+1];
	struct str_identificador * prox;
} tipo_identificador;

typedef struct
{
	tipo_identificador * primeiro;
	tipo_identificador * ultimo;
} tipo_lista_identificadores;

//////////////////////////////////////////////////////////////////////////////

void program();
tipo_lista_comandos * decl1(int num_decl);
tipo_lista_comandos * stmt_list(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2);
tipo_lista_identificadores * ident_list1(char * identificador);
tipo_lista_identificadores * ident_list2(tipo_lista_identificadores * lista_identificadores, char * identificador);
int variable_decl(int tipo, tipo_lista_identificadores * lista_identificadores);
char * identifier(char ident[]);
tipo_lista_comandos * read_stmt(tipo_lista_identificadores * lista_identificadores);
tipo_lista_comandos * factor1(char * identificador);
tipo_lista_comandos * expr_bool_not2(tipo_lista_comandos * lista_comandos);
tipo_lista_comandos * factor_a2(tipo_lista_comandos * lista_comandos);
tipo_lista_comandos * term2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2);
tipo_lista_comandos * expr_bool_and2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2);
tipo_lista_comandos * simple_expr2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2);
tipo_lista_comandos * expr_boolean2(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2);
tipo_lista_comandos * assign_stmt(char * identificador, tipo_lista_comandos * lista_comandos);
tipo_lista_comandos * constant1(int const_int);//int
tipo_lista_comandos * constant2(float const_real);//real
tipo_lista_comandos * constant3(int const_int);//bool
tipo_lista_comandos * constant4(char const_int);//char
int relop(int tipo_rel);
tipo_lista_comandos * expression2(tipo_lista_comandos * lista_esq, int tipo_rel, tipo_lista_comandos * lista_dir);
tipo_lista_comandos * if_stmt1(tipo_lista_comandos * lista_condition, tipo_lista_comandos * lista_then);
tipo_lista_comandos * if_stmt2(tipo_lista_comandos * lista_condition, tipo_lista_comandos * lista_then,
								tipo_lista_comandos * lista_else);
tipo_lista_lista_comandos * expr_list1(tipo_lista_comandos * lista_expression);
tipo_lista_lista_comandos * expr_list2(tipo_lista_lista_comandos * expr_list, tipo_lista_comandos * expression);
tipo_lista_comandos * write_stmt(tipo_lista_lista_comandos * lista_lista_comandos);
tipo_lista_comandos * while_stmt(tipo_lista_comandos * conditions, tipo_lista_comandos * stmts);

//////////////////////////////////////////////////////////////////////////////

tipo_lista_comandos * inicializa_lista_comandos();
tipo_lista_comandos * concatena_listas_comandos(tipo_lista_comandos * lista1, tipo_lista_comandos * lista2);
void libera_lista_comandos(tipo_lista_comandos * lista_comandos);
void insere_comando(tipo_lista_comandos * lista_comandos, char nome_comando[]);
tipo_comando * inicializa_comando(char nome_comando[]);

tipo_lista_lista_comandos * inicializa_lista_lista_comandos();
void libera_lista_lista_comandos(tipo_lista_lista_comandos * lista_lista_comandos);
tipo_lista_lista_comandos * concatena_listas_lista_comandos(tipo_lista_lista_comandos * lista1, tipo_lista_lista_comandos * lista2);
void insere_lista_comandos(tipo_lista_lista_comandos * lista_lista_comandos, tipo_lista_comandos * lista_comandos);

tipo_lista_identificadores * inicializa_lista_identificadores();
void libera_lista_identificadores(tipo_lista_identificadores * lista_identificadores);
void insere_identificador(tipo_lista_identificadores * lista_identificadores, char * nome_identificador);
void seta_tipo_lista(tipo_lista_comandos * lista, int tipo);
int verifica_tipos(tipo_lista_comandos * lista_esq, tipo_lista_comandos * lista_dir, int tipo);

#endif /* ESTRUTURAS_SEMANTICAS_H_INCLUDED */
