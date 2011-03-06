CC=gcc
CFLAGS=-s -Wall -O3
SRC=nifreadertest.c utils.c niff.c nif_reader2.c
OBJS=nifreadertest.o utils.o niff.o nif_reader2.o


nifreadertest : $(OBJS)

#nifreadertest.o : utils.o niff.o nif_reader2.o
#nif_reader2.o : utils.o niff.o

CODEGEN_SRC=convert2c.c utils.c
CODEGEN_BINARY=-o convert2c

codegen :
	$(CC) $(CFLAGS) $(CODEGEN_SRC) $(CODEGEN_BINARY)

.PHONY : clean
clean :
	rm -f nifreadertest nifreadertest.exe $(OBJS) *~
