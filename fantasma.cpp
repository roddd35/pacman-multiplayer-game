#include "fantasma.h"

Fantasma::Fantasma(){
    this->posX = 24;
    this->posY = 3;
}

int Fantasma::getX(){
    return this->posX;
}

int Fantasma::getY(){
    return this->posY;
}

void Fantasma::setX(int x){
    this->posX = x;
}

void Fantasma::setY(int y){
    this->posY = y;
}

void Fantasma::movimentaAleatorio(){
    
}