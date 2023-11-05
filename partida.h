#pragma once 

#include "fantasma.h"
#include "pacman.h"
#include "tabuleiro.h"

#include <iostream>
#include <random>

class Partida{
    private:
        int perdeu;
        // int pacDots;
    public:
        void setPerdeu(int val);
        void jogo(Pacman p, Fantasma f, Tabuleiro tab);
        char decideDirecao(int dir);
        Partida();
        ~Partida();
};