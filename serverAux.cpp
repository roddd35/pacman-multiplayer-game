#include "serverAux.h"
#include "serverDef.h"

/* Rodar o TCP com thread */
void* handleTCP(void* arg){
    ThreadArgs* args = new ThreadArgs;
    args = (ThreadArgs*)arg;
    int socket = *(int*)args->connfd;
    char recvline[MAXLINE];
    ssize_t n;

    // std::cout << "[Uma conexão aberta]" << std::endl;
    while((n=read(socket, recvline, MAXLINE)) > 0){
        recvline[n]=0;
        if ((fputs(recvline,stdout)) == EOF) {
            perror("fputs :( \n");
            exit(6);
        }
        write(socket, recvline, strlen(recvline));
    }
    /* funcoes basicas do servidor vem aqui */

    close(socket);
    delete args;
    pthread_exit(nullptr);
}

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