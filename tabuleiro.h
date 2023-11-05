#pragma once

#include "pacman.h"
#include "fantasma.h"
#include <iostream>
#include <string>

#define FANTASMA 1
#define PLAYER 2

#define rowTab 5
#define colTab 27

using namespace std;

class Tabuleiro {
    private:
        int pacDots;
        char tab[rowTab][colTab];

    public:
        Tabuleiro();
        int getPacdots();
        void imprimeTabuleiro();
        void posPerda(int x, int y);
        Pacman movimentaPacman(char mov, Pacman p, char labAux[5][27]);
        Fantasma movimentaFantasma(char mov, Fantasma f, char labAux[5][27], int sprite);
};
