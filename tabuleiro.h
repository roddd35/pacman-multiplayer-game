#pragma once

#include "pacman.h"
#include "fantasma.h"
#include <iostream>
#include <string>

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
        Pacman movimentaPacman(char mov, Pacman p, char labAux[5][27]);
};
