#include "tabuleiro.h"
#include "pacman.h"
#include "fantasma.h"
#include "partida.h"

int main(){
    Tabuleiro tab = Tabuleiro();
    // tab->imprimeTabuleiro();
    Pacman pacman = Pacman();
    Fantasma fantasma = Fantasma();
    Partida match = Partida();

    match.jogo(pacman, fantasma, tab);

    exit(0);
}