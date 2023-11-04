#include "tabuleiro.h"

Tabuleiro::Tabuleiro(){
    int pos = 0;
    int qtdPacdots = 0;
    string tabStr = "******.**... .....**.************.**.*******.**.************.**.*.. ..*.**.******..... ....*.....*.......F..******.**.*.. ..*.**.******";

    for(int i = 0; i < rowTab; i++){
        for(int j = 0; j < colTab; j++){
            this->tab[i][j] = tabStr[pos];
            if(tabStr[pos] == '.')
                qtdPacdots++;
            pos++;
        }
    }
    this->tab[2][13] = 'C'; // inicializar o pacman no centro do tabuleiro
    this->pacDots = qtdPacdots;
}

void Tabuleiro::imprimeTabuleiro(){
    for(int i = 0; i < rowTab; i++){
        for(int j = 0; j < colTab; j++)
            cout << this->tab[i][j];
        cout << "\n";
    }
    cout << "\n";
}

int Tabuleiro::getPacdots(){
    int qtdPacdots = 0;
    for(int i = 0; i < rowTab; i++)
        for(int j = 0; j < colTab; j++)
            if(this->tab[i][j] == '.')
                qtdPacdots++;
    return qtdPacdots;
}

Pacman Tabuleiro::movimentaPacman(char mov, Pacman p, char labAux[5][27]){
    // -*-*-*- VERTICAL -*-*-*-
    if(mov == 'W' || mov == 'w'){
        // movimentacao comum
        if(this->tab[p.getY() - 1][p.getX()] != '*' && p.getY() - 1 >= 0){
            if(this->tab[p.getY() - 1][p.getX()] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY() - 1][p.getX()] = 'C';
            p.setY(p.getY() - 1);
        }
        // aparecer o pacman do outro lado do tabuleiro
        else if(p.getY() - 1 < 0){
            if(this->tab[rowTab - 1][p.getX()] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[rowTab - 1][p.getX()] = 'C';
            p.setY(rowTab - 1);
        }
    }

    if(mov == 'S' || mov == 's'){
        // movimentacao comum
        if(this->tab[p.getY() + 1][p.getX()] != '*' && p.getY() + 1 < rowTab){
            if(this->tab[p.getY() + 1][p.getX()] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY() + 1][p.getX()] = 'C';
            p.setY(p.getY() + 1);
        }
        // aparecer o pacman do outro lado do tabuleiro
        else if(p.getY() + 1 >= rowTab){
            if(this->tab[0][p.getX()] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[0][p.getX()] = 'C';
            p.setY(0);
        }
    }

    // -*-*-*- HORIZONTAL -*-*-*-
    if(mov == 'D' || mov == 'd'){
        // movimentacao comum
        if(this->tab[p.getY()][p.getX() + 1] != '*' && p.getX() < colTab){
            if(this->tab[p.getY()][p.getX() + 1] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX() + 1] = 'C';
            p.setX(p.getX() + 1);
        }
        // aparecer o pacman do outro lado do tabuleiro
        else if(p.getX() + 1 >= colTab){
            if(this->tab[p.getY()][0] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][0] = 'C';
            p.setX(0);
        }
    }

    if(mov == 'A' || mov == 'a'){
        // movimentacao comum
        if(this->tab[p.getY()][p.getX() - 1] != '*' && p.getX() > 0){
            if(this->tab[p.getY()][p.getX() - 1] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX() - 1] = 'C';
            p.setX(p.getX() - 1);
        }
        // aparecer o pacman do outro lado do tabuleiro
        else if(p.getX() <= 0){
            if(this->tab[p.getY()][colTab - 1] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][colTab - 1] = 'C';
            p.setX(0);
        }
    }

    return p;
}