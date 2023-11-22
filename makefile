PP=g++
CFLAGS= -Wall -g
OTHERS= tabuleiro.cpp pacman.cpp fantasma.cpp partida.cpp

################################

OBJS=tabuleiro.o pacman.o fantasma.o partida.o
SERVEROBJS=servidor.o serverAux.o server.o

MAINSERVER=servidor.cpp
MAINCLIENT=cliente.cpp
MAIN=main.cpp

################################

.PHONY: clean

all: ep

server: svr

client: clt

tabuleiro.o: tabuleiro.h
pacman.o: pacman.h
fantasma.o: fantasma.h
partida.o: partida.h
serverAux.o: serverAux.h
server.o: server.h
servidor.o: servidor.cpp
cliente.o: cliente.cpp
main.o: main.cpp


ep: ${OBJS} 
		${PP} ${CFLAGS} -o main ${MAIN} ${OTHERS}
		@rm -rf *.o *.dSYM .vscode .DS_Store

svr: ${SERVEROBJS}
		${PP} ${CFLAGS} -o servidor ${MAINSERVER} 
		@rm -rf *.o *.dSYM .vscode .DS_Store

clt: cliente.o
		${PP} ${CFLAGS} -o cliente ${MAINCLIENT}
		@rm -rf *.o *.dSYM .vscode .DS_Store

clean:
	@rm -rf main servidor cliente .vscode .DS_Store *.dSYM