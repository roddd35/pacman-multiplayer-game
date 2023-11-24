#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "serverAux.h"
#include "serverDef.h"

// Thread TCP
void* handleTCP(void* arg){
    ThreadArgs* args = new ThreadArgs;
    args = (ThreadArgs*)arg;
    int socket = *(int*)args->connfd;
    // int flag = 1;
    // char recvline[MAXLINE];
    // ssize_t size;

    write(socket, "Bem vindo. Digite um comando: ", 31);
    processCommand(socket);

    close(socket);
    delete args;
    pthread_exit(nullptr);
}

// Thread UDP
void* handleUDP(void* arg){
    ThreadArgs* args = new ThreadArgs;
    args = (ThreadArgs*)arg;

    int socket = *(int*)args->connfd;
    char recvline[MAXLINE];
    struct sockaddr_in clientAddress = (struct sockaddr_in)args->clientAddress;

    ssize_t n;
    socklen_t clientAddressLength = sizeof(clientAddress);

    // std::cout << "[Uma conexão aberta]" << std::endl;
    /* funcoes basicas do servidor vem aqui */
    while (1) {
        n = recvfrom(socket, recvline, MAXLINE, 0, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if(n == -1){
            pthread_exit(nullptr);
            return nullptr;
        }
        recvline[n] = 0;
        if ((fputs(recvline,stdout)) == EOF) {
            perror("fputs error");
            exit (1);
        }
        sendto(socket, recvline, n, 0, (struct sockaddr*)&clientAddress, clientAddressLength);
    }

    close(socket);
    delete (int*)arg;
    delete args;
    pthread_exit(nullptr);
}

// processar o comando digitado pelo cliente
void processCommand(int sockfd){
    int op = 0;
    char recvOp[5];
    char recvline[MAXLINE];
    ssize_t size;
    
    // ideia: para cada operação possível, vamos definir um numero
    // novo: 1, senha: 2, ...
    // cada numero ocupa 4 bytes na mensagem
    // estrutura da mensagem: [operacao] [tamanho primeiro trecho] [primeiro trecho] [tamanho segundo trecho] [segundo trecho]
    // o tamanho de cada trecho também deve ser enviado em 4 bytes (eu espero)
    size = read(sockfd, recvOp, 5);
    op = std::atoi(recvOp);

    // OPERAÇÕES
    // novo usuário
    if(op == 1){
        // ler o tamanho do username e limpar buffer
        int userLen;
        size = read(sockfd, recvline, 5);
        userLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << userLen << std::endl;

        // ler o username e limpar buffer
        char username[userLen];
        size = read(sockfd, recvline, userLen);
        for(int i = 0; i < userLen; i++)
            username[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // std::cout << username << std::endl;

        // ler o tamanho da senha e limpar buffer
        int passLen;
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << passLen << std::endl;

        // ler a senha e limpar buffer
        char pass[passLen];
        size = read(sockfd, recvline, passLen);
        for(int i = 0; i < passLen; i++)
            pass[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // std::cout << pass << std::endl;

        // fazer o registro no arquivo de log
        // guardar o nome, a senha, o socket(?) a pontuacao
    }
    // mudar senha
    else if(op == 2){
        // ler o tamanho da senha antiga
        int passLen;
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << passLen << std::endl;

        // ler a senha antiga e verificar se ela bate com o sistema
        char oldPass[passLen];
        size = read(sockfd, recvline, passLen);
        for(int i = 0; i < passLen; i++)
            oldPass[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // std::cout << oldPass << std::endl;

        // ler o tamanho da senha nova
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << passLen << std::endl;

        // ler a senha nova e registrar no sistema
        char newPass[passLen];
        size = read(sockfd, recvline, passLen);
        for(int i = 0; i < passLen; i++)
            newPass[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // std::cout << newPass << std::endl;
    }
    // logar
    else if(op == 3){
        // ler o tamanho do username e limpar buffer
        int userLen;
        size = read(sockfd, recvline, 5);
        userLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // ler o username e limpar buffer
        char username[userLen];
        size = read(sockfd, recvline, userLen);
        for(int i = 0; i < userLen; i++)
            username[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // ler o tamanho da senha e limpar buffer
        int passLen;
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // ler a senha e limpar buffer
        char pass[passLen];
        size = read(sockfd, recvline, passLen);
        for(int i = 0; i < passLen; i++)
            pass[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // verificar se o login e senha estao corretos
    }
    // tabela de pontuacao de todos usuarios
    else if(op == 4){

    }
    // usuarios conectados
    else if(op == 5){
        
    }
    // iniciar partida como pacman
    else if(op == 6){
        
    }
    // entrar em outra partida
    else if(op == 7){
        int challengeLen;
        size = read(sockfd, recvline, 5);
        challengeLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << challengeLen << std::endl;

        // ler o desafio
        char challenge[challengeLen];
        size = read(sockfd, recvline, challengeLen);
        for(int i = 0; i < challengeLen; i++)
            challenge[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // std::cout << challenge << std::endl;
    }
    // desloga
    else if(op == 8){
        
    }
    // finaliza execução do cliente
    else if(atoi(recvOp) == 9){
        std::cout << "[Cliente " << sockfd << " desconectado]" << std::endl;
        close(sockfd);
    }
}