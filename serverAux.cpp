#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "serverAux.h"
#include "serverDef.h"

std::vector<clientData*> data;

// Thread TCP
void* handleTCP(void* arg){
    ThreadArgs* args = new ThreadArgs;
    args = (ThreadArgs*)arg;
    int socket = *(int*)args->connfd;
    int ret = 1;
    // int flag = 1;
    // char recvline[MAXLINE];
    // ssize_t size;

    write(socket, "Bem vindo. Digite um comando: ", 31);
    // verifica se o usuario parou o servidor com ^C
    while(ret == 1){
        // std::signal(SIGINT, manipuladorSinal);
        ret = processCommand(socket);
    }

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
// FAZER OS WRITES AQUI PRO CLIENTE E TALS
// ESCREVER A STRUCTURE TODA NO ARQUIVO
// DAR LOAD NA STRUCTURE QND INICIA O SERVER
int processCommand(int sockfd){
    int op = 0;
    int res = 1;
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
        std::string username;
        size = read(sockfd, recvline, userLen);
        username = recvline;
        memset(recvline, 0, sizeof(recvline));

        // std::cout << username << std::endl;

        // ler o tamanho da senha e limpar buffer
        int passLen;
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << passLen << std::endl;

        // ler a senha e limpar buffer
        std::string pass;
        size = read(sockfd, recvline, passLen);
        pass = recvline;
        memset(recvline, 0, sizeof(recvline));

        // std::cout << pass << std::endl;

        // verifica se o usuario ja existe
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->username == username){
                res = 0;
                write(sockfd, "0", 1);
                break;
            }
        }
        if(res){
            // guardar o nome, a senha, o socket(?) a pontuacao
            clientData* cd = new clientData;
            cd->username = username;
            cd->password = pass;
            cd->clientSock = sockfd;
            cd->isConnected = true;
            cd->isPlaying = false;
            cd->allTimeScore = 0;
            data.push_back(cd);
            write(sockfd, "1", 1);
            writeFile("Cliente cadastrado");
        }
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
        std::string oldPass;
        size = read(sockfd, recvline, passLen);
        oldPass = recvline;
        memset(recvline, 0, sizeof(recvline));

        // std::cout << oldPass << std::endl;

        // ler o tamanho da senha nova
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << passLen << std::endl;

        // ler a senha nova
        std::string newPass;
        size = read(sockfd, recvline, passLen);
        newPass = recvline;
        memset(recvline, 0, sizeof(recvline));

        // std::cout << newPass << std::endl;

        // buscar entre os clientes cadastrados e mudar senha
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->clientSock == sockfd){
                // std::cout << data[i]->password << " " << oldPass << std::endl;
                if(data[i]->password == oldPass){
                    data[i]->password = newPass;
                    write(sockfd, "1", 1);
                    writeFile("Senha de cliente alterada");
                    // reescrever arquivo com todos os itens do struct depois
                }
                else
                    write(sockfd, "0", 1);
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
        std::string username;
        size = read(sockfd, recvline, userLen);
        username = recvline;
        memset(recvline, 0, sizeof(recvline));

        // ler o tamanho da senha e limpar buffer
        int passLen;
        size = read(sockfd, recvline, 5);
        passLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // ler a senha e limpar buffer
        std::string pass;
        size = read(sockfd, recvline, passLen);
        pass = recvline;
        memset(recvline, 0, sizeof(recvline));

        // verificar se o cliente existe
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->username == username && data[i]->password == pass && !data[i]->isConnected){
                data[i]->isConnected = true;
                write(sockfd, "1", 1);
                writeFile("Cliente conectado");
                existe = true;
            }
        }
        // fazer um write aqui talvez
        if(!existe)
            write(sockfd, "0", 1);
    }
    // tabela de pontuacao de todos usuarios
    else if(op == 4){
        std::ostringstream oss;
        for (int i = 0; i < (int)data.size(); i++)
            oss << data[i]->username << ": " << data[i]->allTimeScore << "\n";

        std::string resultString = oss.str();
        write(sockfd, resultString.c_str(), resultString.size());
    }
    // usuarios conectados
    else if(op == 5){
        std::ostringstream oss;
        for (int i = 0; i < (int)data.size(); i++)
            if(data[i]->isConnected)
                oss << data[i]->username << "-> " << data[i]->isPlaying << "\n";

        std::string resultString = oss.str();
        write(sockfd, resultString.c_str(), resultString.size());
    }
    // iniciar partida como pacman
    else if(op == 6){
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->clientSock == sockfd && data[i]->isConnected){
                data[i]->isPlaying = true;
                write(sockfd, "1", 1);
                writeFile("Cliente iniciou partida");
            }
        }
        write(sockfd, "0", 1);
    }
    // entrar em outra partida
    else if(op == 7){
        int flag = 0;
        int challengeLen;
        size = read(sockfd, recvline, 5);
        challengeLen = std::atoi(recvline);
        memset(recvline, 0, sizeof(recvline));

        // std::cout << challengeLen << std::endl;

        // ler o desafio
        std::string challenge;
        size = read(sockfd, recvline, challengeLen);
        challenge = recvline;
        memset(recvline, 0, sizeof(recvline));

        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->clientSock == sockfd){
                for(int j = 0; j < (int)data.size(); j++){
                    if(data[i]->username == challenge){
                        flag = 1;
                        data[i]->isPlaying = true;
                        write(sockfd, "1", 1);
                        writeFile("Cliente se conectou a uma partida");
                    }
                }
            }
        }
        if(!flag)      
            write(sockfd, "0", 1);
    }
    // desloga
    else if(op == 8){
        for(int i = 0; i < (int)data.size(); i++)
            if(data[i]->clientSock == sockfd)
                data[i]->isConnected = false;
        writeFile("Cliente se desconectou");
    }
    // finaliza execução do cliente
    // arrumar esse comando aqui, encerrando o servidor solo
    else if(atoi(recvOp) == 9){
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->clientSock == sockfd){
                data[i]->isConnected = false;
                write(sockfd, "[Cliente desconectado]", 22);
                writeFile("Cliente desconectado");
            }
        }
        return 0;
    }
    return 1;
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