#pragma once

#include "serverAux.h"

class Server{
    private:
        std::vector<int> ports;

    public:
        Server(std::vector<int> p);
        ~Server();
        int runServer();
};