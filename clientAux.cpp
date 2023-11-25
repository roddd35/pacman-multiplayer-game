#include "clientAux.h"

void read_begin(int sock){
    ssize_t size;
    char recvline[SIZEMAX];

    if((size = read(sock, recvline, 31)) != -1){
        if(!strcmp(recvline,"exit\n") || !strcmp(recvline, "exit")){
            // desconectar cliente
            std::cout << "[Servidor Encerrado]" << std::endl;
            return;
        }
        else
            std::cout << recvline << std::endl;
    }
    memset(recvline, 0, sizeof(recvline));
}

// fazer o write de um comando
// estrutura da mensagem:
// [comando] [tamanho da primeira parte] [primeira parte] [tamanho da segunda parte] [segunda parte]
void write_command(int sock, std::string command, char ipCliente[INET_ADDRSTRLEN]){
    int op;
    std::string aux;
    char numString[5];
    char msg[MAXLINE];

    write(sock, ipCliente, INET_ADDRSTRLEN);

    if(command == "novo"){
        // escrever o comando (numero de 4 bytes)
        op = 1;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o tamanho do primeiro trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)aux.length());
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));
        
        // escrever o primeiro trecho
        for(int i = 0; i < (int)aux.length(); i++)
            msg[i] = aux[i];
        std::cout << msg << " ";
        write(sock, msg, aux.length());
        memset(msg, 0, sizeof(msg));

        // escrever o tamanho do segundo trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)aux.length());
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o segundo trecho
        for(int i = 0; i < (int)aux.length(); i++)
            msg[i] = aux[i];
        std::cout << msg << std::endl;
        write(sock, msg, aux.length());
        memset(msg, 0, sizeof(msg));

        read(sock, msg, 1);
        if(atoi(msg) == 1)
            std::cout << "[Cliente cadastrado]" << std::endl;
        else
            std::cout << "[Ops, esse cliente já existe]" << std::endl;
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "senha"){
        // escrever o comando (numero de 4 bytes)
        op = 2;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o tamanho do primeiro trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)aux.length());
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o primeiro trecho
        for(int i = 0; i < (int)aux.length(); i++)
            msg[i] = aux[i];
        std:: cout << msg << " ";
        write(sock, msg, aux.length());
        memset(msg, 0, sizeof(msg));

        // escrever o tamanho do segundo trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)aux.length());
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o segundo trecho
        for(int i = 0; i < (int)aux.length(); i++)
            msg[i] = aux[i];
        std::cout << msg << std::endl;
        write(sock, msg, aux.length());
        memset(msg, 0, sizeof(msg));

        read(sock, msg, 1);
        if(atoi(msg) == 1)
            std::cout << "[Senha alterada com sucesso]" << std::endl;
        if(atoi(msg) == 0)
            std::cout << "[Senha anterior incorreta]" << std::endl;
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "entra"){
        // escrever o comando (numero de 4 bytes)
        op = 3;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o tamanho do primeiro trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)aux.length());
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o primeiro trecho
        for(int i = 0; i < (int)aux.length(); i++)
            msg[i] = aux[i];
        write(sock, msg, (int)aux.length());
        memset(msg, 0, sizeof(msg));

        // escrever o tamanho do segundo trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)aux.length());
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o segundo trecho
        for(int i = 0; i < (int)aux.length(); i++)
            msg[i] = aux[i];
        write(sock, msg, (int)aux.length());
        memset(msg, 0, sizeof(msg));


        read(sock, msg, 1);
        if(atoi(msg) == 1)
            std::cout << "[Usuário logado com sucesso]" << std::endl;
        else
            std::cout << "[Usuário ou senha incorreto, ou usuário já logado]" << std::endl;
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "lideres"){
        // escrever o comando (numero de 4 bytes)
        op = 4;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        read(sock, msg, sizeof(msg));
        std::cout << "Tabela de pontuação: " << std::endl;
        std::cout << msg << std::endl;
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "l"){
        // escrever o comando (numero de 4 bytes)
        op = 5;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        read(sock, msg, sizeof(msg));
        std::cout << "Usuários conectados e jogando: " << std::endl;
        std::cout << msg << std::endl;
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "inicia"){
        // escrever o comando (numero de 4 bytes)
        op = 6;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        read(sock, msg, 1);
        if(atoi(msg) == 1){
            //iniciar jogo
            std::cout << "Jogo iniciando..." << std::endl;
        }
        else
            std::cout << "Por favor, se autentique para poder jogar!" << std::endl;
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "desafio"){
        // escrever o comando (numero de 4 bytes)
        op = 7;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o tamanho do oponente
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)aux.length());
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o oponente
        for(int i = 0; i < (int)aux.length(); i++)
            msg[i] = aux[i];
        std::cout << msg << std::endl;
        write(sock, msg, aux.length());
        memset(msg, 0, sizeof(msg));

        read(sock, msg, 1);
        if(atoi(msg) == 1)
            std::cout << "Jogo iniciando..." << std::endl;
        else
            std::cout << "Esse jogador não está jogando!" << std::endl;
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "sai"){
        // escrever o comando (numero de 4 bytes)
        op = 8;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        std::cout << "[Usuário deslogado]" << std::endl;
    }

    else if(command == "tchau"){
        // escrever o comando (numero de 4 bytes)
        op = 9;
        snprintf(numString, sizeof(numString), "%04d", op);
        std::cout << "tchau" << std::endl;
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        read(sock, msg, 22);
        std::cout << msg << std::endl;
        memset(numString, 0, sizeof(numString));
    }
}