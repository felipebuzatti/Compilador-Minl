MODULES = main.o mepa.o program.o
BIN_MEPA = MEPA.elf

CFLAGS = -Wall -g3

all: $(MODULES)
	g++  *.o -o ./../$(BIN_MEPA)
	rm -rf *.o

.cpp.o:
	g++ -c $(CFLAGS) $<

clean:
	rm -rf *.o ./../$(BIN_MEPA) *~
