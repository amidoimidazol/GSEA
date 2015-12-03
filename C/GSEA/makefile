CC=g++
CFLAGS=-Wall
LIB=-L/home/dell/gsl/lib
INC=-I/home/dell/gsl/include -I./header

all: GSEA
program: GSEA.o
program.o: hypergeometric.cpp GSEA.cpp FDR.cpp

clean:
	rm -f GSEA GSEA.o
run: program
	./program