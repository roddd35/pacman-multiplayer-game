#include "serverDef.h"
#include "serverAux.h"

/*-*-*-*-*-* Construtor *-*-*-*-*-*/
Server::Server(std::vector<int> p){
    for(int i = 0; i < (int)p.size(); i++)
        this->ports.push_back(p[i]);
}

/*-*-*-*-*-* Destrutor *-*-*-*-*-*/
Server::~Server(){
    
}

/*-*-*-*-*-* Running server Function *-*-*-*-*-*/
int Server::runServer(){
    ll tUDP = 0;
    ll tTCP = 0;
    pid_t childpid;
    pthread_t udpThreads[QTDTHREADS];
    pthread_t tcpThreads[QTDTHREADS];
    std::vector<int> udpSocks(this->ports.size());
    std::vector<int> tcpSocks(this->ports.size());
    std::vector<struct sockaddr_in> servaddr(this->ports.size());

    // Leitura/armazenamento do Socket e Porta
    for(int i = 0; i < (int)this->ports.size(); i++){
        udpSocks[i] = socket(AF_INET, SOCK_DGRAM, 0);
        tcpSocks[i] = socket(AF_INET, SOCK_STREAM, 0);

        // Verificar se leu os sockets corretamente
        if (udpSocks[i] == -1) {
            std::cerr << "[ERRO]: Criação socket UDP" << std::endl;
            exit(0);
        }

        if (tcpSocks[i] == -1) {
            std::cerr << "[ERRO]: Criação socket TCP" << std::endl;
            exit(0);
        }

        // Servidor de Echo
        bzero(&servaddr[i], sizeof(servaddr[i]));
        servaddr[i].sin_family = AF_INET;
        servaddr[i].sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr[i].sin_port = htons(this->ports[i]);

        // Sockets de reuso
        int enableReuse = 1;
        if (setsockopt(tcpSocks[i], SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) == -1) {
            std::cerr << "[ERRO]: Socket Reuse TCP" << std::endl;
            exit(1);
        }

        if (setsockopt(udpSocks[i], SOL_SOCKET, SO_REUSEADDR, &enableReuse, sizeof(int)) == -1) {
            std::cerr << "[ERRO]: Socket Reuse UDP" << std::endl;
            exit(1);
        }

        // Binds
        if (bind(tcpSocks[i], (struct sockaddr*)& servaddr[i], sizeof(servaddr[i])) == -1) {
            std::cerr << "[ERRO]: bind" << std::endl;
            exit(1);
        }

        if (bind(udpSocks[i], (struct sockaddr*)& servaddr[i], sizeof(servaddr[i])) == -1) {
            std::cerr << "[ERRO]: bind" << std::endl;
            exit(1);
        }

        // TCP ouve por conexões
        if (listen(tcpSocks[i], 1) == -1) {
            std::cerr << "[ERRO]: listen" << std::endl;
            exit(1);
        }
    }

    std::cout << "[Servidor no ar]: Aguardando conexões nas portas: ";
    writeLogF("Servidor iniciado", " ", " ");
    for(int i = 0; i < (int)this->ports.size(); i++)
        std::cout << this->ports[i] << " ";
    std::cout << std::endl;

    for(;;){
        for(int i = 0; i < (int)this->ports.size(); i++){
            // Conexões UDP
            std::thread([&](){
                int* udpClientSock = new int;
                *udpClientSock = udpSocks[i];

                ThreadArgs* udpArgs = new ThreadArgs;
                udpArgs->connfd = udpClientSock;
                udpArgs->clientAddress = servaddr[i];

                // Thread para conexão UDP
                if((childpid = pthread_create(&udpThreads[tUDP], nullptr, handleUDP, udpArgs)) == 0)
                    tUDP += 1;
                else{
                    std::cerr << "[ERROR]: creating UDP Thread" << std::endl;
                    delete udpClientSock;
                    delete udpArgs;
                }

            }).detach();

            // Conexões TCP
            std::thread([&](){
                int* tcpClientSock = new int;

                *tcpClientSock = accept(tcpSocks[i], nullptr, nullptr);

                ThreadArgs* tcpArgs = new ThreadArgs;
                tcpArgs->connfd = tcpClientSock;
                tcpArgs->port = this->ports[i];

                // Thread para conexão TCP
                if((childpid = pthread_create(&tcpThreads[tTCP], nullptr, handleTCP, tcpArgs)) == 0){
                    tTCP += 1;
                }
                else{
                    std::cerr << "[ERROR]: creating TCP Thread" << std::endl;
                    delete tcpClientSock;
                    delete tcpArgs;
                }
            }).detach();
        }
    }
    for (int i = 0; i < tUDP; i++)
        pthread_join(udpThreads[i], nullptr);

    for (int i = 0; i < tTCP; i++)
        pthread_join(tcpThreads[i], nullptr);

    for (int i = 0; i < (int)this->ports.size(); i++){
        close(tcpSocks[i]);
        close(udpSocks[i]);
    }

    return 0;
}