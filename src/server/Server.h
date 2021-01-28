#pragma once

#include <map>
#include <list>
#include <vector>
#include <chrono>

#include <netinet/in.h> 

#include "Data.h"
#include "WorkerManager.h"

class Server
{
public:
    Server(std::map<std::string, Data>&& data, int mode, int port, int numberWorker);

    bool setupListening();
    void setup();
    void start();

private:
    std::map<std::string, Data> m_dataSet;
    WorkerManager m_workerManager;
    int m_port;
    int m_mode;

    int m_server_fd;
    struct sockaddr_in m_address;
};

