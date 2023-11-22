#pragma once

#include "serverAux.h"
#include <vector>

class Server{
    private:
        std::vector<int> ports;

    public:
        Server(std::vector<int> p);
        ~Server();
        int runServer();
};