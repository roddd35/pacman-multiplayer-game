#include "serverAux.h"
#include "serverDef.h"

/*  PARA COMPILAR O SERVIDOR:
 *  gcc -Wall -o servidor servidor.cpp
 *  ./servidor UDP 127.0.0.1 8010
 *  ./servidor TCP 127.0.0.1 11100 
 */

int main (int argc, char **argv) {
    std::vector<int> ports;
   
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