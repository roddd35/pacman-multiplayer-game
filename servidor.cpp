#include "serverAux.h"
#include "server.h"

/*  PARA COMPILAR O SERVIDOR:
 *  gcc -Wall -o servidor servidor.cpp
 *  ./servidor UDP 127.0.0.1 8010
 *  ./servidor TCP 127.0.0.1 11100 
 */

int main (int argc, char **argv) {
    ll t = 0;
    int connfd;
    int listenfd;
    int dadosRemotoLen;
    std::vector<int> ports;
    struct sockaddr_in servaddr;    // estrutura de socket necessaria para UDP e TCP
    struct sockaddr_in dadosRemoto; // estrutura de socket necessaria para UDP
    char recvline[MAXLINE + 1];
    char enderecoRemoto[MAXDATASIZE + 1];
    pthread_t threads[QTDTHREADS];
    pid_t childpid;
    ssize_t n;
   
    // Recebeu ao menos uma porta
    if (argc < 2) {
        fprintf(stderr,"Uso: %s <Portas>\n",argv[0]);
        exit(1);
    }
    
    // Portas dadas como strings, converter para int e armazenar
    for(int i = 1; i < argc; i++)
        ports.push_back(std::stoi(argv[i]));   

    Server server(ports);

    server.runServer();

    exit(0);
}

/* Rodar o TCP com thread */
void* handleTCP(void* arg){
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    int socket = *args->connfd;
    ssize_t n;

    std::cout << "[Uma conexão aberta]" << std::endl;
    /* funcoes basicas do servidor vem aqui */

    close(socket);
    delete args;
    pthread_exit(nullptr);
}

void* handleUDP(void* arg){
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    int socket = *args->connfd;
    ssize_t n;
    struct sockaddr_in clientAddress = (struct sockaddr_in)args->clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    printf("[Uma conexão aberta]\n");
    /* funcoes basicas do servidor vem aqui */

    close(socket);
    delete args;
    pthread_exit(nullptr);
}