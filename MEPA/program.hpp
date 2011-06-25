#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

#define P_LENGHT 4096 		/* Tamanho maximo do programa */
#define L_LENGHT 16 		/* Tamanho maximo do label */
#define L_NUMBER 128 		/* Quantidade de labels */
#define OP_LENGHT 5 		/* Tamanho do opcode */
#define OP_NUMBER 50 		/* Quantidade de opcodes */
#define NOT_FOUND -1 		/* Item nao encontrado */
#define NOT_USED -INT_MAX 	/* Para garantir que o campo foi iniciado */
#define MAX_PARAMETROS 3 	/* Numero maximo de parametros de uma funcao */

typedef struct{
    int op; /* opcode */
    char cmd[OP_LENGHT]; /* comando */
    int type; /* 0 = int, 1 = float e 2 = char */
	int f[MAX_PARAMETROS]; /* parametro 1, 2 e 3 */
}Cmd;

typedef struct{
    char label[L_LENGHT];
    int value;
}Label;

class Program
{
    Cmd programa[P_LENGHT]; /* Armazena as instrucoes */
    int i;  /* Contador de programa */
    int n; /* Tamanho do programa */
    int binario; /* Gera codigo binario ou nao */
    
    void load(char *input); /* Carrega o programa */
    void adLabel(Label *tabela, char *label, int nlinha, int ind);
    int getLabel(Label *tabela, char *label, int nlabel);
    void adcmd(char *line, int nlinha, int nlabel, Label *tabela);
    float int2float( int* n );
    int float2int( float* f );

    public:
    Program(char *input, int binario); /* Construtor */
    int getop(char *op); /* Pega o codigo da operacao */
    void setI(int p); /* Muda o contador */
    int  next(); /* Pega a proxima instrucao */
    int get_field(int f); /* Retorna o valor do parametro f */
    void imprime_binario(); /* Imprime representacao binaria em binary.in */
    void imprime(); /* imprime o programa */
    int getI(); /* pega o contador de programa */
    void print_instrucao(int k); /* Imprime a instrucao corrente */
};



#endif

