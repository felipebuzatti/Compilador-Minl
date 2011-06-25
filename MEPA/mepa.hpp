#ifndef MEPA_H
#define MEPA_H

#include <string>
#include <iostream>
#include <vector>

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "program.hpp"

using namespace std;

#define M_LENGHT 4096 /* Tamanho maximo da Pilha */
#define D_LENGHT 512  /* Tamanhao maximo do display */
#define PARA 49

class Mepa
{
	int d[D_LENGHT]; /* Display */
    int m[M_LENGHT]; /* Pilha de inteiros */
    int s;           /* Topo da pilha */
    Program *p;      /* Carrega o programa */
    int detalha;     /* flag para debugar o programa */

    /* Funcao para debug passo a passo */
    void debug();
    
    /* Funcoes MEPA */
    void AMEM();
    void ARMI(); 
    void ARMP();
    void ARMZ(); 
    void CHPP();
    void CHPR();
    void CMAF(); // ( >= float )
    void CMAG(); // ( >= int )
    void CMDF(); // ( != float )
    void CMDG(); // ( != int )
    void CMEF(); // ( <= float )
    void CMEG(); // ( <= int )
    void CMIF(); // ( == float )
    void CMIG(); // ( == int )
    void CMMA(); // ( > int )
    void CMMF(); // ( > float )
    void CMME(); // ( < int )
    void CMNF(); // ( < float )
    void CONJ();
    void CRCT();
    void CRCF();
    void CREG();
    void CREN();
    void CRVI();
    void CRVL();
    void CRVP();
    void DISJ();
    void DIVF();
    void DIVI();
    void DMEM();
    void DSVF();
    void DSVS();
    void ENTR();
    void IMPC();
    void IMPF();
    void IMPR();
    void INPP();
    void INVF();
    void INVR();
    void LEIT();
    void LEIF();
    void MULF();
    void MULT();
    void NEGA();
    void RTPR();
    void SOMA();
    void SOMF();
    void SUBT();
    void SUBF();

    public:
    Mepa( char *input, int det, int bin ); /* construtor do interpletador */
    ~Mepa();                               /* destrutuor */
	int exec();                            /* executa o programa */
    void dump_pilha( int k );              /* imprime as primeiras k+1 posicoes */
    void dump_display( int k );            /* imprime as primeiras k+1 posicoes */
    float int2float( int *n );             /* converte int para float */
    int float2int( float *f );             /* converte float para int */
};
	
#endif
