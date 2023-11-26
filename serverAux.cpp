#pragma GCC diagnostic ignored "-Wunused-but-set-variable"  // evitar esse warning porque declaramos variáveis dentro de condicional
#include "serverAux.h"
#include "serverDef.h"

bool isLoaded = false;
std::vector<clientData*> data;

// Thread TCP
void* handleTCP(void* arg){
    ThreadArgs* args = new ThreadArgs;
    args = (ThreadArgs*)arg;
    int socket = *(int*)args->connfd;
    int ret = 1;

    // carregar os arquivos do sistema
    if(!isLoaded){
        loadFile();
        isLoaded = true;
    }

    write(socket, "Bem vindo. Digite um comando: ", 31);
    // verifica se o usuario parou o servidor com ^C
    while(ret == 1){
        // std::signal(SIGINT, manipuladorSinal);
        ret = processCommand(socket);
    }

    // close(socket);
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
int processCommand(int sockfd){
    int op = 0;
    int res = 1;
    int tamIP = 0;
    char recvOp[5];
    char recvIP[MAXLINE];
    char recvline[MAXLINE];
    ssize_t size;

    memset(recvline, 0, sizeof(recvline));
    memset(recvOp, 0, sizeof(recvOp));
    // ideia: para cada operação possível, vamos definir um numero
    // novo: 1, senha: 2, ...
    // cada numero ocupa 4 bytes na mensagem
    // estrutura da mensagem: [operacao] [tamanho primeiro trecho] [primeiro trecho] [tamanho segundo trecho] [segundo trecho]
    // o tamanho de cada trecho também deve ser enviado em 4 bytes (eu espero)

    // ler ip tamanho IP
    size = read(sockfd, recvIP, 5);
    tamIP = atoi(recvIP);
    memset(recvIP, 0, sizeof(recvIP));

    size = read(sockfd, recvIP, tamIP);
    std::string ip = recvIP;
    memset(recvIP, 0, sizeof(recvIP));

    size = read(sockfd, recvOp, 5);
    op = atoi(recvOp);

    // OPERAÇÕES
    // novo usuário
    if(op == 1){
        std::cout << "entrou na op " << std::endl;
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
            cd->isConnected = false;
            cd->isPlaying = false;
            cd->allTimeScore = 0;
            data.push_back(cd);
            writeClientDataF();
            write(sockfd, "1", 1);
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
                if(data[i]->password == oldPass){
                    data[i]->password = newPass;
                    writeClientDataF();
                    write(sockfd, "1", 1);
                    break;
                }
                else
                    write(sockfd, "0", 1);
            }
        }
        
    }
    // logar
    else if(op == 3){
        bool existe = false;
        int userLen;

        // ler o tamanho do username e limpar buffer
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
                data[i]->clientSock = sockfd;
                write(sockfd, "1", 1);
                writeLogF("Cliente logado!", data[i]->username, ip);
                existe = true;
            }
        }
        if(!existe){
            writeLogF("Cliente não logado!", username, ip);
            write(sockfd, "0", 1);
        }
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
                writeLogF("Cliente iniciou partida!", data[i]->username, ip);
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
                    if(data[j]->username == challenge){
                        flag = 1;
                        data[i]->isPlaying = true;
                        write(sockfd, "1", 1);
                        writeLogF("Cliente se conectou a uma partida", data[i]->username, ip);
                        break;
                    }
                }
            }
        }
        if(!flag)      
            write(sockfd, "0", 1);
    }
    // desloga
    else if(op == 8){
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->clientSock == sockfd && data[i]->isConnected){
                writeLogF("Cliente se desconectou", data[i]->username, ip);
                data[i]->isConnected = false;
            }
        }
    }
    // finaliza execução do cliente
    // arrumar esse comando aqui, encerrando o servidor sozinho
    else if(atoi(recvOp) == 9){
        for(int i = 0; i < (int)data.size(); i++){
            if(data[i]->clientSock == sockfd){
                data[i]->isConnected = false;
                write(sockfd, "[Cliente desconectado]", 22);
                writeLogF("Cliente se desconectou", data[i]->username, ip);
            }
        }
        close(sockfd);
        return 0;
    }
    return 1;
}

void writeLogF(std::string sLog, std::string sUser, std::string sIP){
    std::ofstream arq("logFile.txt", std::ios::app);

    if(arq.is_open()){
        if(sUser != " " && sIP != " ")
            arq << "[" << getCurrentTime() << "] " << sUser << "/" << sIP << ": " << sLog << std::endl;
        else if(sUser == " " || sIP == " ")
            arq << "[" << getCurrentTime() << "] " << sLog << std::endl;
        else if(sIP != " " && sUser == " ")
            arq << "[" << getCurrentTime() << "] " << sIP << ": " << sLog << std::endl;
        arq.close();
    }
    else
        std::cerr << "[Erro] salvar dados no arquivo de Log" << std::endl;
}

std::string getCurrentTime(){
    std::time_t time = std::time(0);

    std::tm* localTime = std::localtime(&time);

    std::ostringstream datetime;
    datetime << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    return datetime.str();
}

void writeClientDataF(){
    std::ofstream arquivo("dados.csv");

    if (arquivo.is_open()) {
        for(int i = 0; i < (int)data.size(); i++)
            arquivo << data[i]->allTimeScore << "," << data[i]->username << "," << data[i]->password << "\n";

        arquivo.close();
    } 
    else
        std::cerr << "Erro ao abrir o arquivo para escrita." << std::endl;
}

std::vector<std::string> split(const std::string &s, char delimiter){
    std::vector<std::string> tokens;
    std::istringstream tokenStream(s);
    std::string token;
    while (std::getline(tokenStream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

void loadFile(){
    std::ifstream arquivo("dados.csv");

    if (arquivo.is_open()) {
        std::string linha;        
        while (std::getline(arquivo, linha)) {
            std::vector<std::string> campos = split(linha, ',');

            if (campos.size() == 3) {
                clientData* novoCliente = new clientData;
                novoCliente->allTimeScore = stoi(campos[0]);
                novoCliente->clientSock = -1;
                novoCliente->isConnected = false;
                novoCliente->isPlaying = false;
                novoCliente->username = campos[1];
                novoCliente->password = campos[2];

                data.push_back(novoCliente);
            } 
            else
                std::cerr << "Erro: Linha inválida no arquivo CSV." << std::endl;
        }
        arquivo.close();
    } 
    else 
        std::cerr << "Erro ao abrir o arquivo para leitura." << std::endl;
}