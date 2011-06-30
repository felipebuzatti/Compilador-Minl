#As pastas src e MEPA têm seus próprios "Makefile". Esse Makefile aqui apenas centraliza os comandos.

DIR_COMP = src
DIR_MEPA = MEPA

BIN_COMP = compilador.elf
BIN_MEPA = MEPA.elf

EXEMPLO = testes/exemplo.txt
SAIDA_EXEMPLO = saida.txt


# =============

.PHONY: make clean run valgrind %.o

# =============

make: $(OBJ)
	@make -f Makefile -C $(DIR_COMP)
	@make -f Makefile -C $(DIR_MEPA)

%.o: %.c %.h
	@echo " --- COMPILANDO OBJETO \"$@\""
	@$(CC) $(CXXFLAGS) $< -c 

clean:
	@make -f Makefile -C $(DIR_COMP) clean
	@make -f Makefile -C $(DIR_MEPA) clean
	rm -f $(OBJ)
	rm -f $(SAIDA_EXEMPLO)
	@clear

run: make
	./$(BIN_COMP) $(EXEMPLO) $(SAIDA_EXEMPLO)
	./$(BIN_MEPA) $(SAIDA_EXEMPLO) 0 0

