#include "serverAux.h"

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
    struct sockaddr_in servaddr;    // estrutura de socket necessaria para UDP e TCP
    struct sockaddr_in dadosRemoto; // estrutura de socket necessaria para UDP
    char recvline[MAXLINE + 1];
    char enderecoRemoto[MAXDATASIZE + 1];
    pthread_t threads[QTDTHREADS];
    pid_t childpid;
    ssize_t n;
   
    if (argc != 4) {
        fprintf(stderr,"Uso: %s <TCP|UDP> <Endereco IP|Nome> <Porta>\n",argv[0]);
        exit(1);
    }

    /*-*-*-*-*-* SOCKET UDP *-*-*-*-*-*/
    if(strcmp(argv[1], "UDP") == 0){
        if ((listenfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
            perror("socket :(\n");
            exit(1);
        }
        /* leitura da porta e conexão */
        bzero(&servaddr, sizeof(servaddr));
        dadosRemotoLen=sizeof(dadosRemoto);
        bzero(&dadosRemoto, dadosRemotoLen);
        servaddr.sin_family      = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port        = htons(atoi(argv[3]));
        if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
            perror("bind :(\n");
            exit(2);
        }

        printf("Servidor UDP iniciado na porta %s.\n", argv[3]);
        /* SERVIDOR */
        for (;;) {
            n = recvfrom(listenfd, recvline, MAXLINE, 0, (struct sockaddr *)&dadosRemoto, (socklen_t *)&dadosRemotoLen);
            printf("Dados do socket remoto: (IP: %s, PORTA: %d enviou mensagem)\n",inet_ntop(AF_INET, &(dadosRemoto.sin_addr).s_addr,enderecoRemoto,sizeof(enderecoRemoto)), ntohs(dadosRemoto.sin_port));

            recvline[n]=0;
            if ((fputs(recvline,stdout)) == EOF) {
                perror("fputs error");
                exit (1);
            }
            /* UDP: O envio de mensagem também tem que ser feito
            * independente, pois não há mais conexão */
            sendto(listenfd,recvline,n,0,(struct sockaddr *)&dadosRemoto,(socklen_t)dadosRemotoLen);
            printf("Dados do socket remoto: (IP: %s, PORTA: %d recebeu um datagrama [mas eu não garanto!])\n",inet_ntop(AF_INET, &(dadosRemoto.sin_addr).s_addr,enderecoRemoto,sizeof(enderecoRemoto)), ntohs(dadosRemoto.sin_port));
	    }
    }

    /*-*-*-*-*-* SOCKET TCP *-*-*-*-*-*/
    else if(strcmp(argv[1], "TCP") == 0){
        if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket :(\n)");
            exit(1);
        }
        /* leitura da porta e conexão */
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family      = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port        = htons(atoi(argv[3]));
        if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
            perror("bind :(\n");
            exit(2);
        }
        /* Socket escutará por conexões */
        /* OBS: Sockets UDP não precisam escutar */
        if (listen(listenfd, LISTENQ) == -1) {
            perror("listen :(\n");
            exit(3);
        }

        /* SERVIDOR
         * OBS: UDP não precisa aceitar a conexão com accept */
        printf("[Servidor TCP aguardando conexões na porta %s]\n", argv[3]);
        printf("[Para finalizar, pressione CTRL+c ou digite kill ou killall]\n");

        for (;;){
            if ((connfd = accept(listenfd, (struct sockaddr *) NULL, NULL)) == -1 ) {
                perror("accept :(\n");
                exit(5);
            }

            /* struct para utilizar threads, ao invés de fork */
            struct ThreadArgs* args = (struct ThreadArgs*)malloc(sizeof(struct ThreadArgs));
            if (args == NULL) {
                perror("Falha na alocação de memória\n");
                close(connfd);
                continue;
            }
            args->connfd = connfd;

            if ((childpid = pthread_create(&threads[t], NULL, makeThread, args)) == 0)
                t++;
            else{
                printf("Não foi possível criar a Thread\n");
                close(connfd);
            }
        }
    }
    else{
        perror("opção inválida de socket!\n");
        exit(1);
    }

    exit(0);
}

/* Rodar o TCP com thread */
void* makeThread(void* arg){
    struct ThreadArgs* args = (struct ThreadArgs*)arg;
    ssize_t n;

    printf("[Uma conexão aberta]\n");
    /* funcoes basicas do servidor vem aqui */

    free(args);
    return NULL;
}