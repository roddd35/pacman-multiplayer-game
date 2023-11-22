#include "clientDef.h"

/*-*-*-*-*-* Construtor *-*-*-*-*-*/
Client::Client(int sock, int port, char protocol[3], char IP[INET_ADDRSTRLEN]){
    this->sockfd = sock;
    this->port = port;
    this->protocol = protocol;
    this->enderecoIPServidor = IP;
}

/*-*-*-*-*-* Destrutor *-*-*-*-*-*/
Client::~Client(){

}
/*-*-*-*-*-* Imprimir *-*-*-*-*-*/
void Client::printParameters(){
    std::cout << "Socket: " << this->sockfd << std::endl;
    std::cout << "Porta: " << this->port << std::endl;
    std::cout << "Protocolo: " << this->protocol << std::endl;
    std::cout << "IP: " << this->enderecoIPServidor << std::endl;
}

/*-*-*-*-*-* Cliente TCP *-*-*-*-*-*/
void Client::handleTCPClient(){
    int	n;
    int dadosLocalLen;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    struct sockaddr_in dadosLocal;

    if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    dadosLocalLen = sizeof(dadosLocal);
    bzero(&dadosLocal, dadosLocalLen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, this->enderecoIPServidor, &servaddr.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }

    if (connect(this->sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    if (getsockname(this->sockfd, (struct sockaddr *) &dadosLocal, (socklen_t *) &dadosLocalLen)) {
        perror("getsockname error");
        exit(1);
    }

    n = 1;
    while (n != 0)  {
        if ((fgets(recvline,MAXLINE,stdin)) != NULL) {
            if (!strcmp(recvline,"exit\n"))
                n = 0;
            else {
                write(this->sockfd, recvline, strlen(recvline));
                n = read(this->sockfd,recvline,MAXLINE);
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
void Client::handleUDPClient(){
    int	n;
    int servaddrLen;
    int dadosLocalLen;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    struct sockaddr_in dadosLocal;

    if ((this->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket error");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    dadosLocalLen = sizeof(dadosLocal);
    bzero(&dadosLocal, dadosLocalLen);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(this->port);

    /* UDP: É necessário passar o tamanho da estrutura para usar na função que
    * envia mensagem */
    servaddrLen=sizeof(servaddr);

    if (inet_pton(AF_INET, enderecoIPServidor, &servaddr.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(1);
    }

    if (getsockname(this->sockfd, (struct sockaddr *) &dadosLocal, (socklen_t *) &dadosLocalLen)) {
        perror("getsockname error");
        exit(1);
    }

    // printParameters();
    n = 1;
    while (n != 0){
        if ((fgets(recvline,MAXLINE,stdin)) != NULL) {
            if (!strcmp(recvline,"exit\n"))
                n = 0;
            else {
                sendto(this->sockfd,recvline,strlen(recvline),0,(struct sockaddr *)&servaddr,(socklen_t)servaddrLen);
                n=recvfrom(this->sockfd,recvline,MAXLINE,0,(struct sockaddr *)&servaddr,(socklen_t *)&servaddrLen);
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