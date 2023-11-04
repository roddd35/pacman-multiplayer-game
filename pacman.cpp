#include "pacman.h"


Pacman::Pacman(){
    this->posX = 14;
    this->posY = 3;
    this->pontos = 0;
    this->status = 1;
}

Pacman::~Pacman(){

}

int Pacman::getX(){
    return this->posX;
}

int Pacman::getY(){
    return this->posY;
}

int Pacman::getPontos(){
    return this->pontos;
}

// mudar aqui quando der, colocar para receber direcao e fazer o movimento com base nisso
void Pacman::setX(int x){
    this->posX = x;
}

void Pacman::setY(int y){
    this->posY = y;
}

void Pacman::somaPontos(){
    this->pontos = this->pontos + 1;
}

void Pacman::setStatus(int status){
    this->status = status;
}