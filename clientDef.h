#pragma once

#include "serverDef.h"
#include "serverAux.h"

#include <string>
#include <cerrno>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 4096

class Client{
    private:
        int port;
        int sockfd;
        char* protocol;
        char* enderecoIPServidor;
    public:
        Client(int sock, int port, char protocol[3], char IP[INET_ADDRSTRLEN]);
        ~Client();
        void printParameters();
        void handleTCPClient();
        void handleUDPClient();
};