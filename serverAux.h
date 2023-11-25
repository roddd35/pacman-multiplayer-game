#pragma once

#include "serverDef.h"
#include <ctime>
#include <thread>
#include <cerrno>
#include <chrono>
#include <vector>
#include <string>
#include <csignal>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <netdb.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define LISTENQ 1
#define MAXDATASIZE 100
#define SIZEMAX 1024
#define MAXLINE 4096
#define QTDTHREADS 150
#define ll long long

struct ThreadArgs {
    int port;
    int* connfd;
    struct sockaddr_in clientAddress;
};

struct clientData {
    int clientSock;
    int allTimeScore;
    bool isPlaying;
    bool isConnected;
    std::string username;
    std::string password;
};

// handleTCP e handleUDP
int processCommand(int sockfd);
void* handleTCP(void* arg);
void* handleUDP(void* arg);
void writeFile(std::string sLog);
void manipuladorSinal(int sinal);
std::string getCurrentTime();