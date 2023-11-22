#include "clientDef.h"

/*  PARA COMPILAR O CLIENTE:
 *  gcc -Wall -o cliente cliente.cpp
 *  ./cliente UDP 127.0.0.1 8010
 *  ./cliente TCP 127.0.0.1 11100
 */

int main(int argc, char **argv) {
    int	sockfd = -1;
    struct hostent *hptr;
    char enderecoIPServidor[INET_ADDRSTRLEN];

    // verificações da entrada
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

    // declarar um objeto cliente
    Client cliente(sockfd, atoi(argv[3]), argv[1], enderecoIPServidor);

    printf("[Conectando no servidor no IP %s]\n", enderecoIPServidor);
    printf("[o comando 'exit' encerra a conexão]\n");

    // Cliente TCP
    if(strcmp(argv[1], "TCP") == 0)
        cliente.handleTCPClient();

    // Cliente UDP
    if(strcmp(argv[1], "UDP") == 0)
        cliente.handleUDPClient();
    exit(0);
}