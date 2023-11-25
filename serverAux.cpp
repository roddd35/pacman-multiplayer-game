#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "serverAux.h"
#include "serverDef.h"

std::vector<clientData*> data;

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
        writeFile("Cliente cadastrado");

        // guardar o nome, a senha, o socket(?) a pontuacao
        clientData* cd = new clientData;
        cd->username = username;
        cd->password = pass;
        cd->clientSock = sockfd;
        cd->isConnected = true;
        cd->allTimeScore = 0;
        data.push_back(cd);
    }
    // mudar senha
    else if(op == 2){
        // ler o tamanho da senha antiga
        int passLen;
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << passLen << std::endl;

        // ler a senha antiga
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

        // ler a senha nova
        char newPass[passLen];
        size = read(sockfd, recvline, passLen);
        for(int i = 0; i < passLen; i++)
            newPass[i] = recvline[i];
        memset(recvline, 0, sizeof(recvline));

        // std::cout << newPass << std::endl;

        // buscar entre os clientes cadastrados e mudar senha
        // colocar isso em uma funcao
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->clientSock == sockfd){
                if(data[i]->password == oldPass){
                    data[i]->password = newPass;
                    writeFile("Senha de cliente alterada");
                    // reescrever arquivo com todos os itens do struct depois
                }
                else
                    std::cout << "Senha antiga incorreta!" << std::endl;
            }
        }
        
    }
    // logar
    else if(op == 3){
        bool existe = false;
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

        // verificar se o cliente existe
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->username == username && data[i]->password == pass){
                data[i]->isConnected = true;
                writeFile("Cliente conectado");
                existe = true;
            }
        }
        // fazer um write aqui talvez
        if(!existe)
            std::cout << "Cliente não cadastrado" << std::endl;
    }
    // tabela de pontuacao de todos usuarios
    else if(op == 4){
        std::cout << "Tabela de Pontuação:" << std::endl;
        for(int i = 0; i < (int)data.size(); i++)
            std::cout << data[i]->username << ": " << data[i]->allTimeScore << std::endl;
    }
    // usuarios conectados
    else if(op == 5){
        std::cout << "Usuários conectados:" << std::endl;
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->isConnected)
                std::cout << data[i]->username << std::endl;
        }
    }
    // iniciar partida como pacman
    else if(op == 6){
        writeFile("Cliente iniciou partida");
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
        writeFile("Cliente se conectou a uma partida");
        // std::cout << challenge << std::endl;
    }
    // desloga
    else if(op == 8){
        for(int i = 0; i < (int)data.size(); i++)
            if(data[i]->clientSock == sockfd)
                data[i]->isConnected = false;
        writeFile("Cliente se desconectou");
    }
    // finaliza execução do cliente
    else if(atoi(recvOp) == 9){
        for(int i = 0; i < (int)data.size(); i++)
            if(data[i]->clientSock == sockfd)
                data[i]->isConnected = false;
        std::cout << "[Cliente " << sockfd << " desconectado]" << std::endl;
        writeFile("Cliente desconectado");
        close(sockfd);
    }
}

void writeFile(std::string sLog){
    std::ofstream arq("logFile.txt", std::ios::app);

    if(arq.is_open()){
        arq << "[" << getCurrentTime() << "] " << sLog << std::endl;
        arq.close();
    }
    else
        std::cerr << "[Erro] salvar dados no arquivo de Log" << std::endl;
}

std::string getCurrentTime(){
    std::time_t time = std::time(0);

    // Obtém a informação do fuso horário local
    std::tm* localTime = std::localtime(&time);

    // Formata a data e hora usando std::put_time
    std::ostringstream datetime;
    datetime << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    return datetime.str();
}