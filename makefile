PP=g++
CFLAGS= -Wall -g
OTHERS= tabuleiro.cpp pacman.cpp fantasma.cpp partida.cpp
SERVEROTHERS= serverAux.cpp serverDef.cpp
CLIENTOTHERS = clientDef.cpp

################################

OBJS=tabuleiro.o pacman.o fantasma.o partida.o
SERVEROBJS=serverDef.o serverAux.o
CLIENTOBJS=clientDef.o

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

clientDef.o: clientDef.h

serverDef.o: serverDef.h
serverAux.o: serverAux.h

servidor.o: servidor.cpp
cliente.o: cliente.cpp
main.o: main.cpp


ep: ${OBJS} 
		${PP} ${CFLAGS} -o main ${MAIN} ${OTHERS}
		@rm -rf *.o *.dSYM .vscode .DS_Store

svr: ${SERVEROBJS}
		${PP} ${CFLAGS} -o servidor ${MAINSERVER} ${SERVEROTHERS}
		@rm -rf *.o *.dSYM .vscode .DS_Store

clt: ${CLIENTOBJS}
		${PP} ${CFLAGS} -o cliente ${MAINCLIENT} ${CLIENTOTHERS}
		@rm -rf *.o *.dSYM .vscode .DS_Store

clean:
	@rm -rf main servidor cliente .vscode .DS_Store *.dSYM