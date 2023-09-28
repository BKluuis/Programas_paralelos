all: main

CC = clang
override CFLAGS += -g -Wno-everything -pthread -lm

SRCS = ./src/auxiliar.c ./src/processos.c ./src/sequencial.c ./src/threads.c
PROGRAMS = $(notdir $(basename $(SRCS)))
BIN = ./bin

main: $(PROGRAMS)

$(PROGRAMS):
	$(CC) $(CFLAGS) $< ./src/$@.c -o $(BIN)/"$@"

debug:
	$(CC) $(CFLAGS) -O0 $(SRCS) -o "$@"

r: clean 
	rm -f $(BIN)/*
	make

clean:
	rm -f ./mat/threads/* ./mat/processos/* ./mat/*.txt
