#pragma once

#include <iostream>
#include <string>

class Fantasma{
    private:
        int posX;
        int posY;
    public:
        Fantasma();
        int getX();
        int getY();
        void setX(int x);
        void setY(int y);
        void movimentaAleatorio();
};