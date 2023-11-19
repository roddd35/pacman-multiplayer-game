#pragma once

#define _GNU_SOURCE
#include <ctime>   
#include <cerrno>  
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
    int connfd;
};

void* makeThread(void* arg);