#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

#define MAXLINE 4096

/*  PARA COMPILAR O CLIENTE:
 *  gcc -Wall -o cliente cliente.cpp
 *  ./cliente UDP 127.0.0.1 8010
 *  ./cliente TCP 127.0.0.1 11100
 */

int main(int argc, char **argv) {
    int	sockfd, n;
    int servaddrLen;
    int dadosLocalLen;
    char recvline[MAXLINE + 1];
    struct hostent *hptr;
    struct sockaddr_in servaddr;
    struct sockaddr_in dadosLocal;
    char enderecoIPServidor[INET_ADDRSTRLEN];

	if (argc != 4) {
        fprintf(stderr,"Uso: %s <TCP|UDP> <Endereco IP|Nome> <Porta>\n", argv[0]);
		exit(1);
	}

    if ((hptr = gethostbyname(argv[2])) == NULL) {
        fprintf(stderr,"gethostbyname :(\n");
        exit(1);
    }
    if (hptr->h_addrtype != AF_INET) {
        fprintf(stderr,"h_addrtype :(\n");
        exit(1);
    }
    if ((inet_ntop(AF_INET, hptr->h_addr_list[0], enderecoIPServidor, sizeof(enderecoIPServidor))) == NULL) {
        fprintf(stderr,"inet_ntop :(\n");
        exit (1);
    }

    printf("[Conectando no servidor no IP %s]\n", enderecoIPServidor);
    printf("[o comando 'exit' encerra a conexão]\n");

    /*-*-*-*-*-* Cliente TCP *-*-*-*-*-*/
    if(strcmp(argv[1], "TCP") == 0){
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket error");
            exit(1);
        }

        bzero(&servaddr, sizeof(servaddr));
        dadosLocalLen = sizeof(dadosLocal);
        bzero(&dadosLocal, dadosLocalLen);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[3]));

        if (inet_pton(AF_INET, enderecoIPServidor, &servaddr.sin_addr) <= 0) {
            perror("inet_pton error");
            exit(1);
        }

        if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
            perror("connect error");
            exit(1);
        }
    
        if (getsockname(sockfd, (struct sockaddr *) &dadosLocal, (socklen_t *) &dadosLocalLen)) {
            perror("getsockname error");
            exit(1);
        }

        n = 1;
        while (n != 0)  {
            if ((fgets(recvline,MAXLINE,stdin)) != NULL) {
                if (!strcmp(recvline,"exit\n"))
                    n = 0;
                else {
                    write(sockfd, recvline, strlen(recvline));
                    n = read(sockfd,recvline,MAXLINE);
                }
            }
            else
                n = 0;
        }

        if (n < 0) {
            perror("read error");
            exit(1);
        }
    }

    /*-*-*-*-*-* Cliente UDP *-*-*-*-*-*/
    else if(strcmp(argv[1], "UDP") == 0){
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
            perror("socket error");
            exit(1);
        }

        bzero(&servaddr, sizeof(servaddr));
        dadosLocalLen = sizeof(dadosLocal);
        bzero(&dadosLocal, dadosLocalLen);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(atoi(argv[3]));

        /* UDP: É necessário passar o tamanho da estrutura para usar na função que
        * envia mensagem */
        servaddrLen=sizeof(servaddr);

        if (inet_pton(AF_INET, enderecoIPServidor, &servaddr.sin_addr) <= 0) {
            perror("inet_pton error");
            exit(1);
        }

        if (getsockname(sockfd, (struct sockaddr *) &dadosLocal, (socklen_t *) &dadosLocalLen)) {
            perror("getsockname error");
            exit(1);
        }

        n = 1;
        while (n != 0){
            if ((fgets(recvline,MAXLINE,stdin)) != NULL) {
                if (!strcmp(recvline,"exit\n"))
                    n = 0;
                else {
                    sendto(sockfd,recvline,strlen(recvline),0,(struct sockaddr *)&servaddr,(socklen_t)servaddrLen);
                    n=recvfrom(sockfd,recvline,MAXLINE,0,(struct sockaddr *)&servaddr,(socklen_t *)&servaddrLen);
                    recvline[n] = 0;
                    if ((fputs(recvline,stdout)) == EOF) {
                    perror("fputs error");
                    exit (1);
                    }
                }
            }
            else
                n = 0;
        }

        if (n < 0) {
            perror("read error");
            exit(1);
        }
    }
   
	exit(0);
}