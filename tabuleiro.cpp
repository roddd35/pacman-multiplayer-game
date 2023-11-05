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

void Tabuleiro::posPerda(int x, int y){
    this->tab[y][x] = 'X';
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
        if(this->tab[p.getY()][p.getX() + 1] != '*' && p.getX() < colTab - 1){
            if(this->tab[p.getY()][p.getX() + 1] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX() + 1] = 'C';
            p.setX(p.getX() + 1);
        }
        // aparecer o pacman do outro lado do tabuleiro
        else if(p.getX() + 1 >= colTab - 1){
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
            cout << "1" << endl;
        }
        // aparecer o pacman do outro lado do tabuleiro
        else if(p.getX() <= 0){
            if(this->tab[p.getY()][colTab - 1] == '.')
                p.somaPontos();
            labAux[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][p.getX()] = ' ';
            this->tab[p.getY()][colTab - 1] = 'C';
            p.setX(colTab - 1);
            cout << "2" << endl;
        }
    }

    return p;
}

Fantasma Tabuleiro::movimentaFantasma(char mov, Fantasma f, char labAux[5][27], int sprite){
    char personagem;
    // definir se é o fantasma padrão ou o fantasma player
    if(sprite == FANTASMA)
        personagem = 'F';
    if(sprite == PLAYER)
        personagem = 'f';

    // -*-*-*- VERTICAL -*-*-*-
    if(mov == 'W' || mov == 'w'){
        // movimentacao comum
        if(this->tab[f.getY() - 1][f.getX()] != '*' && f.getY() - 1 >= 0){
            this->tab[f.getY() - 1][f.getX()] = personagem;
            if(labAux[f.getY() - 1][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setY(f.getY() - 1);
        }
        // aparecer o fantasma do outro lado do tabuleiro
        else if(f.getY() - 1 < 0){
            // this->tab[f.getY()][f.getX()] = ' ';
            this->tab[rowTab - 1][f.getX()] = personagem;
            if(labAux[f.getY()][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setY(rowTab - 1);
        }
    }

    if(mov == 'S' || mov == 's'){
        // movimentacao comum
        if(this->tab[f.getY() + 1][f.getX()] != '*' && f.getY() + 1 < rowTab){
            this->tab[f.getY() + 1][f.getX()] = personagem;
            if(labAux[f.getY()][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setY(f.getY() + 1);
        }
        // aparecer o fantasma do outro lado do tabuleiro
        else if(f.getY() + 1 >= rowTab){
            // this->tab[f.getY()][f.getX()] = ' ';
            this->tab[0][f.getX()] = personagem;
            if(labAux[f.getY()][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setY(0);
        }
    }

    // -*-*-*- HORIZONTAL -*-*-*-
    if(mov == 'D' || mov == 'd'){
        // movimentacao comum
        if(this->tab[f.getY()][f.getX() + 1] != '*' && f.getX() < colTab - 1){
            this->tab[f.getY()][f.getX() + 1] = personagem;
            if(labAux[f.getY()][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setX(f.getX() + 1);
        }
        // aparecer o fantasma do outro lado do tabuleiro
        else if(f.getX() + 1 >= colTab - 1){
            // this->tab[f.getY()][f.getX()] = ' ';
            this->tab[f.getY()][0] = personagem;
            if(labAux[f.getY()][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setX(0);
        }
    }

    if(mov == 'A' || mov == 'a'){
        // movimentacao comum
        if(this->tab[f.getY()][f.getX() - 1] != '*' && f.getX() > 0){
            this->tab[f.getY()][f.getX() - 1] = personagem;
            if(labAux[f.getY()][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setX(f.getX() - 1);
        }
        // aparecer o fantasma do outro lado do tabuleiro
        else if(f.getX() <= 0){
            // this->tab[f.getY()][f.getX()] = ' ';
            this->tab[f.getY()][colTab - 1] = personagem;
            if(labAux[f.getY()][f.getX()] == '.')
                this->tab[f.getY()][f.getX()] = '.';
            else
                this->tab[f.getY()][f.getX()] = ' ';
            f.setX(0);
        }
    }

    return f;
}