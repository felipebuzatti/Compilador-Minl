#include "program.hpp"

/* Converte um int para float */
float Program::int2float( int* n )
{
    float f = *( (float*) (n) );
    return f;
}

/* Converte um float para int */
int Program::float2int( float* f )
{
    int n = *( (int*) (f) );
    return n;
}

/* Pega o contador de programa */
int Program::getI()
{
    return i;
}

/* Imprime a intrucao corrente */
void Program::print_instrucao(int k)
{
    printf("%3d %s(%d)", k, programa[k].cmd, programa[k].op);
	for( int j = 0; j < MAX_PARAMETROS; j++ )
	{
        if( programa[k].f[j] != NOT_USED )
		{
            if ( !programa[k].type ) printf(" %d", programa[k].f[j]);
            else printf(" %f", int2float(&programa[k].f[j]));
        }
    }
    printf("\n");
}

/* Imprime o conteudo do programa */
void Program::imprime()
{
    for( int k = 0; k < n; k++ )
	{
		print_instrucao(k);
    }
}

/* Imprime representacao binaria */
void Program::imprime_binario()
{
    int not_printed;
    ofstream fout( "binary.in", ios::out );
    for ( int k = 0; k < n; k++ )
    {
        not_printed = 1;
        fout << programa[k].op << " ";
        for ( int l = 0; l < MAX_PARAMETROS; l++ )
	    {
            if( programa[k].f[l] != NOT_USED )
    		{
                if ( not_printed )
                {
                    fout << programa[k].type << " ";
                    not_printed = 0;
                }
                fout << programa[k].f[l] << " ";
            }
        }
        fout << endl;
    }
    fout.close();
}

/* Construtor */
Program::Program(char *input, int bin)
{
    i = -1;
    n = 0;
    binario = bin;
    for( int j = 0; j < P_LENGHT; j++ )
	{
        programa[j].type = 0;
		for ( int k = 0; k < MAX_PARAMETROS; k++ )
		{
        	programa[j].f[k] = NOT_USED;
		}
    }
    load(input);
}

/* Carrega o programa, identifica as operacoes, resolve os labels */
void Program::load(char *input)
{
    FILE* f_in = fopen(input, "r");
    FILE *tmp = tmpfile();
    char line[129], *token;
    int l_count; 
    Label labels[L_NUMBER];

    if( !f_in )
	{
        sprintf(line, "Nao foi possivel abrir o arquivo %s\n", input);
        perror(line);
        abort();
    }

    l_count = 0;

    /* Primeiro passo: Monta a tabela de labels */
    while( fgets(line, 128, f_in) )
	{
        if( line[0] == '\n' ) continue;
        line[strlen(line)-1] = '\0';
        token = strtok(line, " ");
		/* Sempre que o primeiro indentificador for diferente de
		   um opcode ele eh considerado um label */
        if( getop(token) == NOT_FOUND)
		{
            adLabel( labels, token, n, l_count );
            l_count++;
        }
        else
		{
            fprintf(tmp, "%s ", token);
        }
        while( ( token = strtok(NULL, " ") ) )
		{
            fprintf(tmp, "%s ", token);
        }

        fprintf(tmp, "\n");
        n++;
    }

    fclose(f_in);
    rewind(tmp);
    n = 0;

    /* Segundo passo: Resolve os labels e opcodes */
    while( fgets(line, 128, tmp) )
	{
        line[strlen(line)-1] = '\0';
        adcmd( line, n, l_count, labels );
        n++;
    }
    
    if ( binario ) imprime_binario();
    
    fclose(tmp);
}

/* Adiciona na area do programa, os comandos, opcode e parametros */
void Program::adcmd(char *line, int nlinha, int nlabel, Label *tabela)
{
    char *token;
    int i;

    token = strtok(line, " ");
    if( !token )
	{
        fprintf(stderr, "Erro na linha %d\n", nlinha);
        abort();
    }
    strcpy(programa[nlinha].cmd, token);
    
    if ( programa[nlinha].cmd[3] == 'F' && strcmp(programa[nlinha].cmd, "DSVF")) 
    {
        programa[nlinha].type = 1;
    }
    else if ( !strcmp(programa[nlinha].cmd, "IMPC") ) 
    {
        programa[nlinha].type = 2;
    }
    
    programa[nlinha].op = getop(token);
    
    if(programa[nlinha].op == NOT_FOUND)
	{
        fprintf(stderr, "macro %s not found, linha %d\n", token, nlinha);
        abort();
    }
    
    i = 0;
    while( ( token = strtok(NULL, " ") ) )
	{
        int l_value = getLabel(tabela, token, nlabel); 
        if( l_value == NOT_FOUND )
		{
            if(isalpha(token[0]))
			{
                fprintf(stderr, "macro %s not defined, linha %d\n", token, nlinha);
                abort();
            }
            if ( !programa[nlinha].type ) programa[nlinha].f[i] = atoi(token);
            else 
            {
                float real = atof(token);
                programa[nlinha].f[i] = float2int(&real);
            }
        }
        else
		{
            programa[nlinha].f[i] = l_value;
        }
        i++;
    }
}

/* Pega o opcode correspondente a operacao */
int Program::getop(char *op)
{                
    char opcode[OP_NUMBER][OP_LENGHT] = { "AMEM", "ARMI", "ARMP", "ARMZ", "CHPP", "CHPR", 
        "CMAF", "CMAG", "CMDF", "CMDG", "CMEF", "CMEG", "CMIF", "CMIG", "CMMA", "CMMF", "CMME", "CMNF",
        "CONJ", "CRCT", "CRCF", "CREG", "CREN", "CRVI", "CRVL", "CVRP", "DISJ", "DIVF", "DIVI", "DMEM",
        "DSVF", "DSVS", "ENTR", "IMPC", "IMPF", "IMPR", "INPP", "INVF", "INVR", "LEIT", "LEIF", "MULF", 
        "MULT", "NEGA", "RTPR", "SOMA", "SOMF", "SUBT", "SUBF", "PARA"
    };
    int i = 0;
    
    while( i < OP_NUMBER && strcmp(opcode[i], op) ) i++;
	if( i == OP_NUMBER ) /* opcode nao encotrado */
	{
    	i = NOT_FOUND; 
    }

    return i;
}

/* Adiciona o Label na tabela de labels */
void Program::adLabel(Label *tabela, char *label, int nlinha, int ind)
{
    strcpy(tabela[ind].label, label);
    tabela[ind].value = nlinha;
}

/* Pega o endereco do label */
int Program::getLabel(Label *tabela, char *label, int nlabel)
{
    int i = 0;
    while( i < nlabel && strcmp(tabela[i].label, label) ) i++;
	if( i == nlabel ) /* O label nao foi encontrado */
	{
        return NOT_FOUND;
    }
    return tabela[i].value;
}

/* Muda o valor do contador de programa para p */
void Program::setI( int p )
{
    i = p;
}

/* Pega a proxima instrucao e incrementa o contador de programa */
int Program::next()
{
	i++;
    assert( i >= 0 && i < n ); /* Verifica se pode acessar a area */

    return programa[i].op;
}

/* Pega o parametro f da instrucao i */
int Program::get_field( int f )
{
    assert( i >= 0 && i < n ); /* Verifica se pode acessar a area */
	assert( f >= 0  && f < MAX_PARAMETROS ); /* Verifica o indice do parametro */
    assert( programa[i].f[f] != NOT_USED ); /* Teste se o campo foi iniciado */

    return programa[i].f[f];
}

