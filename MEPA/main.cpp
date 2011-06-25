#include <stdio.h>
#include "mepa.hpp"

int main(int argc, char **argv){

    Mepa *m;
	
    if(argc < 4)
	{
        printf("usage: <input> <debug> <binario>\n");
        exit(-1);
    }

    m = new Mepa( argv[1], atoi(argv[2]), atoi(argv[3]) );
    m->exec();
    delete m;

    return 0;
}

