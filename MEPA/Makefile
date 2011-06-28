CC = g++

MODULES = main.o mepa.o program.o
BIN_MEPA = MEPA.elf

CFLAGS = -Wall -g3

# =============

.PHONY: all clean %.o

# =============


all: $(MODULES)
	@echo ""
	@echo " --- COMPILANDO MEPA ---"
	@$(CC) $(CFLAGS) $(MODULES) -lm -o $(BIN_MEPA)
	@echo ""

main.o: main.cpp
	@echo " --- COMPILANDO OBJETO \"$@\""
	@$(CC) $(CFLAGS) $< -c 

%.o: %.cpp %.hpp
	@echo " --- COMPILANDO OBJETO \"$@\""
	@$(CC) $(CFLAGS) $< -c 

clean:
	rm -rf *.o ./../$(BIN_MEPA) *~
