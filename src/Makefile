CC = gcc

CFLAGS = -Wall -g -Wextra

RM = rm -f

OBJ = lexikon.o principal.o parser.tab.o estruturas_semanticas.o \
		floresta.o procedimentos.o

MAIN = ../compilador.elf

ARGS = ../testes/exemplo.txt

# =============

.PHONY: $(MAIN) %.o make clean run valgrind 

# =============

make: parse $(MAIN)

parse: parser.y
	bison -d parser.y

$(MAIN): parse $(OBJ)
	@echo ""
	@echo " --- COMPILANDO COMPILADOR ---"
	@$(CC) $(CFLAGS) $(OBJ) -lm -o $(MAIN)
	@echo ""

parser.tab.o: parser.tab.c parser.tab.h
	@echo "--- COMPILANDO OBJETO \"$@\""
	@$(CC) $(CFLAGS) $< -c

%.o: %.c %.h
	@echo " --- COMPILANDO OBJETO \"$@\""
	@$(CC) $(CFLAGS) $< -c 

clean:
	$(RM) $(MAIN) *.o
	$(RM) parser.tab.h parser.tab.c
	$(RM) *~
	clear

run: $(MAIN)
	@./$(MAIN) $(ARGS)
	
 	
valgrind: $(MAIN)
	@valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./$(MAIN) $(ARGS)

gdb: $(MAIN)
	@gdb --args $(MAIN) $(ARGS)

