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
void write_command(int sock){
    int op;
    std::string command;
    std::string aux;
    char numString[5];
    char msg[MAXLINE];

    // ler o comando
    std::cin >> command;
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
        std::cout << msg << std::endl;
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
        std::cout << msg << std::endl;
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
    }

    else if(command == "entra"){
        // escrever o comando (numero de 4 bytes)
        op = 3;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o tamanho do primeiro trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)sizeof(aux));
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o primeiro trecho
        for(int i = 0; i < (int)sizeof(aux); i++)
            msg[i] = aux[i];
        write(sock, msg, sizeof(aux));
        memset(msg, 0, sizeof(msg));

        // escrever o tamanho do segundo trecho
        std::cin >> aux;
        snprintf(numString, sizeof(numString), "%04d", (int)sizeof(aux));
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));

        // escrever o segundo trecho
        for(int i = 0; i < (int)sizeof(aux); i++)
            msg[i] = aux[i];
        write(sock, msg, sizeof(aux));
        memset(msg, 0, sizeof(msg));
    }

    else if(command == "lideres"){
        // escrever o comando (numero de 4 bytes)
        op = 4;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));
    }

    else if(command == "l"){
        // escrever o comando (numero de 4 bytes)
        op = 5;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));
    }

    else if(command == "inicia"){
        // escrever o comando (numero de 4 bytes)
        op = 6;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));
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
    }

    else if(command == "sai"){
        // escrever o comando (numero de 4 bytes)
        op = 8;
        snprintf(numString, sizeof(numString), "%04d", op);
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));
    }

    else if(command == "tchau"){
        // escrever o comando (numero de 4 bytes)
        op = 9;
        snprintf(numString, sizeof(numString), "%04d", op);
        std::cout << "tchau" << std::endl;
        write(sock, numString, sizeof(numString));
        memset(numString, 0, sizeof(numString));
    }
}