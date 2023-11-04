#pragma once

#include <iostream>
#include <string>

class Pacman{
    private:
        int posX;
        int posY;
        int status; // 1 caso vivo, 0 caso morto
        int pontos; // soma 1 a cada pacdot consumido

    public:
        Pacman();
        ~Pacman();
        int getX();
        int getY();
        int getPontos();
        void setX(int x);
        void setY(int y);
        void somaPontos();
        void setStatus(int status);
};