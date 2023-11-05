PP=g++
CFLAGS= -Wall -g
OTHERS= tabuleiro.cpp pacman.cpp fantasma.cpp partida.cpp

################################

OBJS=tabuleiro.o pacman.o fantasma.o partida.o
MAIN=main.cpp

################################

.PHONY: clean

all: ep

tabuleiro.o: tabuleiro.h
pacman.o: pacman.h
fantasma.o: fantasma.h
partida.o: partida.h
main.o: main.c

ep: ${OBJS} 
		${PP} ${CFLAGS} -o main ${MAIN} ${OTHERS}
		@rm -rf *.o *.dSYM .vscode .DS_Store

clean:
	@rm -rf main