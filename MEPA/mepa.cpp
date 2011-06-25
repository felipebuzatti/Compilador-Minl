#include "mepa.hpp"

/* Converte um int para float */
float Mepa::int2float( int* n )
{
    float f = *( (float*) (n) );
    return f;
}

/* Converte um float para int */
int Mepa::float2int( float* f )
{
    int n = *( (int*) (f) );
    return n;
}

/* Construtor carrega o programa */
Mepa::Mepa( char *input, int det, int bin )
{
    p = new Program(input, bin);
    detalha = det;
    s = -1;
}

/* Destrutor libera o espaco alocado pelo programa */
Mepa::~Mepa()
{
    delete p;
}

/* Imprime o conteudo das k + 1 primeiras posicoes da pilha */
void Mepa::dump_pilha( int k )
{
    assert( k >= 0 && (k+1 ) < P_LENGHT );
	cout << "Conteudo da pilha:" << endl;
    cout << "Pos\tValor" << endl;
    for( int j = 0; j <= k; j++ )
	{
        cout << j << "\t" << m[j] << endl;
    }
}

/* Imprime o conteudo das k + 1 primeiras posicoes do display */
void Mepa::dump_display( int k )
{
    assert( k >= 0 && (k+1) < D_LENGHT );
    cout << "Conteudo do diplay:" << endl;
    cout << "D\tValor" << endl;
    for( int j = 0; j <= k; j++ )
	{
        cout << j << "\t" << d[j] << endl;
    }
}

/* Imprime estado da pilha apos cada instrucao */
void Mepa::debug()
{
    char tmp;
    cout << "\033[33m" "|---------------------------|\n" "\033[0m" "\033[34m";
    p->print_instrucao(p->getI());
    cout << "Topo da pilha = " << s << endl;
    if( s >= 0 )
	{
        dump_pilha(s);
    }
    cout << "\033[33m" "|---------------------------|\n\n" "\033[0m";
    scanf("%c", &tmp);
}

/* Executa o programa */
int Mepa::exec(){

	int op;

    if(detalha)
	{
         p->imprime(); /* Imprime o programa */
    }

    op = p->next();
    while( op != PARA )
	{
        if(detalha) 
		{	
			debug();
		}
        switch(op)
		{
            /* Funcoes MEPA */
            case 0: { AMEM(); break; } 
            case 1: { ARMI(); break; } 
            case 2: { ARMP(); break; }
            case 3: { ARMZ(); break; } 
            case 4: { CHPP(); break; } 
            case 5: { CHPR(); break; }
            case 6: { CMAF(); break; } 
            case 7: { CMAG(); break; } 
            case 8: { CMDF(); break; }
            case 9: { CMDG(); break; } 
            case 10: { CMEF(); break; } 
            case 11: { CMEG(); break; }
            case 12: { CMIF(); break; } 
            case 13: { CMIG(); break; } 
            case 14: { CMMA(); break; } 
            case 15: { CMMF(); break; } 
            case 16: { CMME(); break; } 
            case 17: { CMNF(); break; } 
            case 18: { CONJ(); break; } 
            case 19: { CRCT(); break; } 
            case 20: { CRCF(); break; } 
            case 21: { CREG(); break; }
            case 22: { CREN(); break; }
            case 23: { CRVI(); break; } 
            case 24: { CRVL(); break; }
            case 25: { CRVP(); break; }
            case 26: { DISJ(); break; }
            case 27: { DIVF(); break; } 
            case 28: { DIVI(); break; }
            case 29: { DMEM(); break; } 
            case 30: { DSVF(); break; } 
            case 31: { DSVS(); break; } 
            case 32: { ENTR(); break; } 
            case 33: { IMPC(); break; }
            case 34: { IMPF(); break; }
            case 35: { IMPR(); break; }
            case 36: { INPP(); break; }
            case 37: { INVF(); break; } 
            case 38: { INVR(); break; }
            case 39: { LEIT(); break; }
            case 40: { LEIF(); break; }
            case 41: { MULF(); break; }
            case 42: { MULT(); break; }
            case 43: { NEGA(); break; }
            case 44: { RTPR(); break; }
            case 45: { SOMA(); break; }
            case 46: { SOMF(); break; }
            case 47: { SUBT(); break; }
            case 48: { SUBF(); break; }
            default:{
               cerr << "O programa executou uma operacao invalida." << endl;
               cerr << "i = " << p->getI() << endl; 
               abort();
            }
        }
        op = p->next();
    }
    return 0;
}    

/* Divide o valor anterior ao topo pelo valor no topo da pilha (INT) */
void Mepa::DIVI()
{
    assert( (s-1) > 0 );
    if( !m[s] ) 
	{
        perror("Divisao por zero\n");
        abort();
    }
    m[s-1] = m[s-1] / m[s];
    s = s - 1;
}

/* Divide o valor anterior ao topo pelo valor no topo da pilha (FLOAT) */
void Mepa::DIVF()
{
    assert( (s-1) > 0 );
    float fnum = int2float(&m[s-1]); /* numerador */
    float fden = int2float(&m[s]); /* denominador */
    float quoc = fnum / fden;
    if( fabs(fden) < 1e-10 ) 
	{
        perror("Divisao por zero\n");
        abort();
    }
    m[s-1] = float2int(&quoc);
    s = s - 1;
}

/* Desaloca memoria */
void Mepa::DMEM()
{
    int tmp1 = p->get_field(0);
    s = s - tmp1;
}

/* Carrega referencia display[param_1] + param_2 */
void Mepa::CREN()
{
    int tmp1, tmp2;
    tmp1 = p->get_field(0);
    tmp2 = p->get_field(1);
	s = s + 1;
    m[s] = d[tmp1] + tmp2;
}

/* Salva valor no espaco apontado */
void Mepa::ARMI()
{
    int tmp1, tmp2, ponteiro;
    tmp1 = p->get_field(0);
    tmp2 = p->get_field(1);
    ponteiro = m[d[tmp1] + tmp2];
    m[ponteiro] = m[s];
    s = s - 1;
}

/* Carrega referencia */
void Mepa::CRVI()
{
    int tmp1, tmp2, ponteiro;
    s = s + 1;
    tmp1 = p->get_field(0);
    tmp2 = p->get_field(1);
    ponteiro = m[d[tmp1] + tmp2];
    m[s] = m[ponteiro];
}

/* carrega pilha com nível param_1 (offset = param_2 + topo_pilha) */
void Mepa::CRVP()
{
    int tmp1, tmp2, ponteiro;
    tmp1 = p->get_field(0);
    tmp2 = p->get_field(1);
    ponteiro = m[d[tmp1] + tmp2 + m[s]];
    s = s + 1;
    m[s] = m[ponteiro];
}

/* nível param_1 (offset = param_2 + topo_pilha) = topo_pilha-1 */
void Mepa::ARMP()
{
    int tmp1, tmp2, ponteiro;
    tmp1 = p->get_field(0);
    tmp2 = p->get_field(1);
    ponteiro = m[d[tmp1] + tmp2 + m[s]];
    m[ponteiro] = m[s-1];
}

/* Incrementa o apontador de topo */
void Mepa::AMEM()
{
    int tmp1 = p->get_field(0);
    s = s + tmp1;
}

/* Carrega o valor do topo da pilha em nivel param_1 (offset param_2) */
void Mepa::ARMZ()
{
    int tmp1, tmp2;
    tmp1 = p->get_field(0);
    tmp2 = p->get_field(1);
    m[d[tmp1] + tmp2] = m[s];
    s = s - 1;
}

/* Chama procedimento que e parametro */
void Mepa::CHPP()
{
    int enM, enN; /* Endereco textual do parametro */
    int k; 		  /* Nivel lexico de onde ocorre a chamada */
    int i;        /* Inicio do procedimento */
    int f, tmpf;  /* Guarda o contexto */

    enM = p->get_field(0);
    enN = p->get_field(1);
    k = p->get_field(2);
    m[s+1] = p->getI() + 1;
    m[s+2] = d[k];
    m[s+3] = k;
    s = s + 3;
    i = m[d[enM] + enN];
    p->setI(i - 1);
    tmpf = f = m[d[enM] + enN + 2];
    d[f] = m[d[enM] + enN + 1];

    while(f >= 2)
	{
        d[f-1] = m[d[f] - 1];
        f = f - 1;
    }
    if(detalha)
	{
        dump_display(tmpf);
    }
}

/* Chamar procedimento */
void Mepa::CHPR()
{
    int p_1;   /* Endereco da primeira instrucao do procedimento */
    int k;     /* Nivel do local onde ocorre CHPR */
    int i;     /* Endereco de chpr */
	
    p_1 = p->get_field(0);
    k = p->get_field(1);
    i = p->getI();
    m[s+1] = i + 1;
    m[s+2] = d[k];
    m[s+3] = k;
    s = s + 3;
    p->setI(p_1 - 1);
}

/* Comparar maior (INT)*/
void Mepa::CMMA()
{
    assert(s > 0);
    if( m[s-1] > m[s] ) 
    { 
        m[s-1] = 1; 
    }
    else 
    { 
        m[s-1] = 0; 
    }
    s = s - 1;
}

/* Comparar maior (FLOAT)*/
void Mepa::CMMF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    if( f1 > f2 ) 
    { 
        m[s-1] = 1; 
    }
    else 
    { 
        m[s-1] = 0; 
    }
    s = s - 1;
}

/* Comparar maior ou igual (INT) */
void Mepa::CMAG()
{
	assert(s > 0);
    if( m[s-1] >= m[s] )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar maior ou igual (FLOAT)*/
void Mepa::CMAF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    if( f1 - f2 >= 1e-10 ) 
    { 
        m[s-1] = 1; 
    }
    else 
    { 
        m[s-1] = 0; 
    }
    s = s - 1;
}

/* Comparar diferente (INT) */
void Mepa::CMDG()
{
    assert(s > 0);
    if( m[s-1] != m[s] )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar diferente (FLOAT) */
void Mepa::CMDF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    if( fabs(f1 - f2) > 1e-10 )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar menor ou igual (INT) */
void Mepa::CMEG()
{
    assert(s > 0);
    if( m[s-1] <= m[s] )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar menor ou igual (FLOAT) */
void Mepa::CMEF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    if( f1 - f2 <= 1e-10 )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar igual (INT) */
void Mepa::CMIG()
{
    assert(s > 0);
    if( m[s-1] == m[s] )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar igual (FLOAT) */
void Mepa::CMIF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    if( fabs( f1 - f2 ) < 1e-10 )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar menor (INT) */
void Mepa::CMME()
{
    assert(s > 0);
    if( m[s-1] < m[s] )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Comparar menor (FLOAT) */
void Mepa::CMNF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    if( f1 < f2 )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Conjuncao */
void Mepa::CONJ()
{
    assert(s > 0);
    if( m[s-1] && m[s] )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Carrega constante (INT) */
void Mepa::CRCT()
{
    int tmp;
    s = s + 1;
    tmp = p->get_field(0);
    m[s] = tmp;
}

/* Carrega constante (FLOAT) */
void Mepa::CRCF()
{
    float tmp;
    s = s + 1;
    tmp = p->get_field(0);
    m[s] = float2int(&tmp);
}

/* Carrega endereco generalizado usado para passar como parametro */
void Mepa::CREG()
{
    int p_enter; /* endereco da entrada do procedimento */
    int k;       /* nivel lexico de onde o procedimento foi declarado */
    p_enter = p->get_field(0);
    k = p->get_field(1);
    m[s+1] = p_enter;
    m[s+2] = d[k];
    m[s+3]= k;
    s = s + 3;
}

/* Carrega uma variavel no topo da pilha */
void Mepa::CRVL()
{
    int tmp1, tmp2;
    s = s + 1;
    tmp1 = p->get_field(0);
    tmp2 = p->get_field(1);
    m[s] = m[d[tmp1] + tmp2];
}

/* Disjuncao */
void Mepa::DISJ()
{
    assert(s > 0);
    if( m[s-1] || m[s] )
	{
        m[s-1] = 1;
    }
    else
	{
        m[s-1] = 0;
    }
    s = s - 1;
}

/* Desvia sempre */
void Mepa::DSVS()
{
    int tmp;
    tmp = p->get_field(0) - 1;
    p->setI(tmp);
}

/* Desvia se falso */
void Mepa::DSVF()
{
    int tmp;
    if( !m[s] )
	{
        tmp = p->get_field(0) - 1;
        p->setI(tmp);
    }
    s = s - 1;
}

/* Entrar no procedimento */
void Mepa::ENTR()
{
    int k; /* Nivel do procedimento */
    k = p->get_field(0);
    s = s + 1;
    m[s] = d[k-1]; /* Link estatico */
    d[k] = s + 1;
    if(detalha)
	{
        dump_display(k);
    }
}

/* Imprime (INT) */
void Mepa::IMPR()
{
    cout << m[s] << endl;
    s = s - 1;
}

/* Imprime (FLOAT) */
void Mepa::IMPF()
{
    float f = int2float(&m[s]);
    cout << f << endl;
    s = s - 1;
}

/* Imprime (CHAR) */
void Mepa::IMPC()
{
    char c = m[s];
    cout << c << endl;
    s = s - 1;
}

/* Inicia o apontador de topo e o display */
inline void Mepa::INPP()
{
    s = -1;
    d[0] = 0;
}

/* Inverter sinal (INT) */
inline void Mepa::INVR()
{
    m[s] = -m[s];
}

/* Inverter sinal (FLOAT) */
inline void Mepa::INVF()
{
    float f = int2float(&m[s]);
    f = -f;
    m[s] = float2int(&f);
}

/* Le um valor e o guarda no topo da pilha (INT) */
void Mepa::LEIT()
{
    int tmp;
    s = s + 1;
    cout << "\033[31m" "Entre com um numero: " "\033[0m";
    scanf("%d", &tmp);
    m[s] = tmp;
}

/* Le um valor e o guarda no topo da pilha (FLOAT) */
void Mepa::LEIF()
{
    float tmp;
    s = s + 1;
    cout << "\033[31m" "Entre com um numero: " "\033[0m";
    scanf("%f", &tmp);
    m[s] = float2int(&tmp);
}

/* Multiplica o valor do topo da pilha com um valor anterior (INT) */
void Mepa::MULT()
{
    assert( (s-1) > 0 );
    m[s-1] = m[s-1] * m[s];
    s = s - 1;
}

/* Multiplica o valor do topo da pilha com um valor anterior (FLOAT)*/
void Mepa::MULF()
{
    assert( (s-1) > 0 );
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    float prod = f1 * f2;
    m[s-1] = float2int(&prod);
    s = s - 1;
}

/* Negacao */
inline void Mepa::NEGA()
{
    m[s] = 1 - m[s];
}

/* Retornar de procedimento */
void Mepa::RTPR()
{
    int n_p;     /* Numero de parametros */
    int k, tmpk; /* Nivel do procedimento */
    int i;       /* Endereco de retorno */

    n_p = p->get_field(0);
    tmpk = k = m[s-1];
    d[k] = m[s-2];
    i = m[s-3];
    p->setI(i - 1);
    s = s - (n_p+4);
    while( k >= 2 )
	{
        assert( (d[k]-1) >= 0 );
        d[k-1] = m[d[k] - 1];
        k = k - 1;
    }
    if(detalha)
	{
        dump_display(tmpk);
    }
}

/* Soma o valor no topo da pilha e o da posicao anterior (INT) */
void Mepa::SOMA()
{
    assert(s > 0);
    m[s-1] = m[s-1] + m[s];
    s = s - 1;
}

/* Soma o valor no topo da pilha e o da posicao anterior (FLOAT) */
void Mepa::SOMF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    float sum = f1 + f2;
    m[s-1] = float2int(&sum);
    s = s - 1;
}

/* Subtrai do valor anterior ao topo da pilha o valor do topo (INT) */
void Mepa::SUBT()
{
    assert(s > 0);
    m[s-1] = m[s-1] - m[s];
    s = s - 1;
}

/* Subtrai do valor anterior ao topo da pilha o valor do topo (FLOAT) */
void Mepa::SUBF()
{
    assert(s > 0);
    float f1 = int2float(&m[s-1]);
    float f2 = int2float(&m[s]);
    float dif = f1 - f2;
    m[s-1] = float2int(&dif);
    s = s - 1;
}

