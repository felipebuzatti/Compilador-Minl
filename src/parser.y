%{

#include "lexikon.h"
#include "estruturas_semanticas.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "floresta.h"
#include "procedimentos.h"

void yyerror (char const *s);
int yylex();

%}

%union{
	int unsig_int;
	double unsig_real;
	char identificador[255];
	char char_const;
	tipo_lista_comandos * lista_comandos;
	char * ident;
	tipo_lista_identificadores * lista_identificadores;
	tipo_param * param;
	tipo_lista_param * lista_param;
	tipo_lista_lista_comandos * lista_lista_comandos;
};
%error-verbose

%start program;

%token PROGRAM
%token DECLARE
%token DO
%token ENDPROGRAM 0
%token END
%token INTEGER
%token REAL
%token BOOLEAN
%token CHAR
%token LABEL
%token ARRAY
%token INTEGER_CONST
%token PROCEDURE
%token ATRIBUI ":="
%token IF
%token THEN
%token ELSE
%token WHILE
%token READ
%token WRITE
%token GOTO
%token NOT
%token EQ "="
%token LT "<"
%token LE "<="
%token DIF "!="
%token OR
%token AND
%token RETURN
%token OF
%token BOOL_CONST_T
%token BOOL_CONST_F
%token <unsig_int> UNSIG_INT
%token <unsig_real> UNSIG_REAL
%token <identificador> IDENT
%token <char_const> CHAR_CONST
%type <lista_comandos> proc_body
%type <lista_comandos> block_stmt
%type <lista_comandos> stmt_list
%type <lista_comandos> read_stmt
%type <lista_comandos> unlabelled_stmt
%type <lista_comandos> stmt
%type <lista_comandos> factor
%type <lista_comandos> factor_a
%type <lista_comandos> term
%type <lista_comandos> simple_expr
%type <lista_comandos> expression
%type <lista_comandos> expr_boolean
%type <lista_comandos> expr_bool_and
%type <lista_comandos> expr_bool_not
%type <lista_comandos> assign_stmt
%type <lista_comandos> constant
%type <unsig_int> relop
%type <lista_comandos> if_stmt
%type <lista_comandos> condition
%type <lista_lista_comandos> expr_list
%type <lista_comandos> write_stmt
%type <lista_comandos> while_stmt
%type <lista_comandos> proc_decl
%type <lista_comandos> proc_stmt
%type <ident> identifier
%type <ident> variable
%type <lista_identificadores> ident_list
%type <lista_comandos> decl
%type <unsig_int> variable_decl
%type <lista_comandos> decl_list
%type <unsig_int> simple_type
%type <unsig_int> type
%type <unsig_int> integer_constant
%type <unsig_int> boolean_constant
%type <unsig_int> parameter_type
%type <unsig_real> real_constant
%type <char_const> char_constant
%type <param> parameter_decl
%type <lista_param> formal_list
%type <lista_param> proc_header

%%

program: PROGRAM identifier proc_body ENDPROGRAM	{program($3);}
;

proc_body: block_stmt								{$$ = $1}
;

block_stmt: DECLARE decl_list DO stmt_list END		{$$ = concatena_listas_comandos($2, $4);}
	| DO stmt_list END								{$$ = $2}
;

decl_list: decl ';'									{$$ = $1}
	| decl_list decl ';'							{$$ = concatena_listas_comandos($1, $2);}
;

decl: variable_decl									{$$ = decl1($1);}
	| proc_decl										{$$ = $1}
;

variable_decl: type ident_list						{$$ = variable_decl($1, $2);}
;

ident_list: identifier								{$$ = ident_list1($1);}
	| ident_list ',' identifier						{$$ = ident_list2($1, $3);}
;

type: simple_type									{$$ = $1}
	| array_type									{$$ = 0}
;

simple_type: INTEGER 										{$$ = 0}
	| REAL 											{$$ = 1}
	| BOOLEAN 										{$$ = 2}
	| CHAR 											{$$ = 3}
	| LABEL											{$$ = 4}
;

array_type: ARRAY tamanho OF simple_type
;

tamanho: INTEGER_CONST
;

proc_decl: proc_header proc_body					{$$ = proc_decl($1, $2);}
;

proc_header: PROCEDURE identifier '(' formal_list ')'	{$$ = $4; $$->endereco = proc_header($2, $4);}
;

formal_list: parameter_decl ';'						{$$ = formal_list1($1);}
	| formal_list parameter_decl ';'				{$$ = insere_param($1, $2);}
;

parameter_decl: parameter_type identifier			{$$ = cria_param($1, $2);}
;

parameter_type: type									{$$ = $1}
	| proc_signature								{$$ = 5}
;

proc_signature: PROCEDURE identifier '(' type_list ')'
	| PROCEDURE identifier
;

type_list: parameter_type
	| type_list ',' parameter_type
;

stmt_list: stmt	';'				{$$ = $1}
	| stmt_list stmt ';'	{$$ = stmt_list($1, $2);}
;

stmt: label ':' unlabelled_stmt	{$$ = $3}
	| unlabelled_stmt			{$$ = $1}
;

label: identifier
;

unlabelled_stmt: assign_stmt 					{$$ = $1}
	| if_stmt 									{$$ = $1}
	| while_stmt 								{$$ = $1}
	| read_stmt 								{$$ = $1}
	| write_stmt 								{$$ = $1}
	| goto_stmt 								{$$ = NULL}
	| proc_stmt 								{$$ = $1}
	| return_stmt								{$$ = NULL} 
	| block_stmt								{$$ = NULL}
;

assign_stmt: variable ATRIBUI expr_boolean		{$$ = assign_stmt($1, $3);}
;

variable: identifier 							{$$ = $1}
	| array_element								{$$ = NULL}
;

array_element: identifier '[' expression ']'
;

if_stmt: IF condition THEN stmt_list END				{$$ = if_stmt1($2, $4);}
	| IF condition THEN stmt_list ELSE stmt_list END	{$$ = if_stmt2($2, $4, $6);}
;

condition: expr_boolean								{$$ = $1}
;

while_stmt: WHILE condition DO stmt_list END		{$$ = while_stmt($2, $4);}
;

read_stmt: READ '(' ident_list ')'					{$$ = read_stmt($3);}
;

write_stmt: WRITE '(' expr_list ')'					{$$ = write_stmt($3);}
;

goto_stmt: GOTO label
;

proc_stmt: identifier				{$$ = proc_stmt1($1);}
	| identifier '(' expr_list ')'	{$$ = proc_stmt2($1, $3);}
;

return_stmt: RETURN
;

expr_list: expression				{$$ = expr_list1($1);}
	| expr_list ',' expression		{$$ = expr_list2($1, $3);}
;

expr_boolean: expr_bool_and			{$$ = $1}
	| expr_bool_and OR expr_boolean	{$$ = expr_boolean2($1, $3);}
;

expr_bool_and: expr_bool_not			{$$ = $1}
	| expr_bool_not AND expr_bool_and	{$$ = expr_bool_and2($1, $3);}
;

expr_bool_not: expression			{$$ = $1}
	| NOT expression				{$$ = expr_bool_not2($2);}
;

expression: simple_expr				{$$ = $1}
	| simple_expr relop simple_expr	{$$ = expression2($1, $2, $3);}
;


simple_expr: term				{$$ = $1}
	| simple_expr '+' term	{$$ = simple_expr2($1, $3);}
;

term: factor_a				{$$ = $1}
	| term '*' factor_a	{$$ = term2($1, $3);}
;

factor_a: factor			{$$ = $1}
	| '-' factor			{$$ = factor_a2($2);}
;

factor: variable 			{$$ = factor1($1);}
	| constant				{$$ = $1}
	| '(' expr_boolean ')'	{$$ = $2}
;

relop: EQ 					{$$ = relop(0);}
	| LT  					{$$ = relop(1);}
	| LE  					{$$ = relop(2);}
	| DIF 					{$$ = relop(3);}
;

constant: integer_constant		{$$ = constant1($1);}
	| real_constant 		{$$ = constant2($1);}
	| char_constant 		{$$ = constant4($1);}
	| boolean_constant		{$$ = constant3($1);}
;

boolean_constant: BOOL_CONST_T	{$$ = 1}
	| BOOL_CONST_F				{$$ = 0}
;

integer_constant: UNSIG_INT	{$$ = $1}
;

char_constant: CHAR_CONST	{$$ = $1}
;

real_constant: UNSIG_REAL	{$$ = $1}
;

identifier: IDENT			{$$ = identifier($1);}
;

%%

int linha = 0;

int yylex(){
	static item_lex * item_cabeca = NULL;
	if (item_cabeca == NULL){
		item_cabeca = (item_lex *)malloc(sizeof(item_lex));
		item_cabeca->prox = NULL;
	}
	
	item_lex * item_aux = item_cabeca->prox;
	
	do
	{
		if(item_aux == NULL && analise_lex(stdin, item_cabeca) == 0)
			return ENDPROGRAM;
		linha+=1;
	} while (item_cabeca->prox == NULL);

	if(item_aux == NULL){
		item_aux = item_cabeca->prox;
	}
		

	if (item_aux->value != NULL){
		if (item_aux->token == UNSIG_REAL){
			float * valor = (float*)item_aux->value;
			yylval.unsig_real = *valor;
		}
		else if (item_aux->token == IDENT)
		{
			char * valor = (char*)item_aux->value;
			strcpy(yylval.identificador, valor);
		}
		else if (item_aux->token == UNSIG_INT)
		{
			int * valor = (int*)item_aux->value;
			yylval.unsig_int = *valor;
		}
		else
		{
			char * valor = (char*)item_aux->value;
			yylval.char_const = *valor;
		}
	}
	
	item_cabeca = item_aux;
	
	return item_aux->token;
}


void yyerror (char const *s)
     {
       fprintf (stderr, "%s na linha %d\n", s, linha);
     }
