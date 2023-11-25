#pragma once

#include "clientDef.h"
#include "serverDef.h"
#include "serverAux.h"

void read_begin(int sock);
void write_command(int sock, std::string command, char ipCliente[INET_ADDRSTRLEN]);