#pragma once

#define _GNU_SOURCE
#include "server.h"
#include <ctime>
#include <thread>
#include <cerrno>
#include <vector>
#include <string>
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
#define MAXLINE 4096
#define QTDTHREADS 150
#define ll long long

struct ThreadArgs {
    int port;
    int* connfd;
    struct sockaddr_in clientAddress;
};

// handleTCP e handleUDP
void* handleTCP(void* arg);
void* handleUDP(void* arg);