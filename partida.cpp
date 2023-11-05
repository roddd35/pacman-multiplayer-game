#include "partida.h"

Partida::Partida(){
    this->perdeu = 0;
}

Partida::~Partida(){

}

void Partida::setPerdeu(int val){
    if(val == 1)
        this->perdeu = 1;
    if(val == 0)
        this->perdeu = 0;
}

void Partida::jogo(Pacman p, Fantasma f, Tabuleiro tab){
    int auxDir;
    int pos = 0;
    int maxPontos;
    int qtdPacdots = 0;
    char direcao;
    char labAux[5][27];
    string tabStr = "******.**... .....**.************.**.*******.**.************.**.*.. ..*.**.******..... ....*.....*....... ..******.**.*.. ..*.**.******";

    // criar tabuleiro auxiliar
    for(int i = 0; i < rowTab; i++){
        for(int j = 0; j < colTab; j++){
            labAux[i][j] = tabStr[pos];
            pos++;
        }
    }

    // iniciar parametros
    qtdPacdots = tab.getPacdots();
    maxPontos = qtdPacdots;
    // p.setPontos(qtdPacdots);
    
    // rodar o jogo
    while(!this->perdeu){
        // player escolhe movimento do pacman
        tab.imprimeTabuleiro();
        cout << "Direção (a -> esquerda, d -> direita, w -> cima, s -> baixo): ";
        cin >> direcao;
        p = tab.movimentaPacman(direcao, p, labAux);

        // movimento aleatorio do fantasma F
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<>dist(1,4);

        auxDir = dist(gen);
        direcao = decideDirecao(auxDir);
        f = tab.movimentaFantasma(direcao, f, labAux, FANTASMA);

        // ver como fazer o movimento do PLAYER aqui

        // verifica se o jogo acabou por pontuacao
        if(p.getPontos() == maxPontos){
            tab.imprimeTabuleiro();
            cout << "Parabéns! Pontos: " << p.getPontos() << endl;
            break;
        }

        // verifica se o jogo acabou por colisao com fantasma
        if(p.getX() == f.getX() && p.getY() == f.getY()){
            tab.posPerda(p.getX(), p.getY());
            tab.imprimeTabuleiro();
            setPerdeu(1);
            cout << "Fim de jogo! Pontos = " << p.getPontos() << " / " << maxPontos << endl;
        }
    }
}

char Partida::decideDirecao(int dir){
    char direcao;
    if(dir == 1)
        direcao = 'W';
    if(dir == 2)
        direcao = 'A';
    if(dir == 3)
        direcao = 'S';
    if(dir == 4)
        direcao = 'D';
    return direcao;
}