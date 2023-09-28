all: main

CC = clang
override CFLAGS += -O3 -Wno-everything -pthread -lm

SRCS = ./src/auxiliar.c ./src/processos.c ./src/sequencial.c ./src/threads.c
PROGRAMS = $(notdir $(basename $(SRCS)))
BIN = ./bin

main: $(PROGRAMS)

$(PROGRAMS):
	$(CC) $(CFLAGS) $< ./src/$@.c -o $(BIN)/"$@"

debug:
	$(CC) $(CFLAGS) -O0 $(SRCS) -o "$@"

r: b clean main

b:  
	rm -f $(BIN)/*

clean:
	rm -f ./mat/threads/* ./mat/processos/* ./mat/*.txt
