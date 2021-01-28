#include <boost/algorithm/string.hpp>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <iostream>

#include "Server.h"
#include "TimeToolKit.h"
#include "Input.h"
#include "Task.h"

constexpr int request_symbol = 0;
constexpr int request_time = 2;
constexpr int size_max_request = 32;

#define ms(X) std::chrono::duration_cast<std::chrono::microseconds>(X)

Server::Server(std::map<std::string, Data>&& data, int mode, int port, int numberWorker)
    : m_dataSet(std::move(data)),
    m_workerManager(numberWorker),
    m_port(port),
    m_mode(mode)
{
    m_workerManager.start();
} 

void Server::setup()
{
    int opt = 1; 

    // Creating socket file descriptor 
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        std::cerr << "Unable to create socket." << std::endl; 
        exit(EXIT_FAILURE); 
    } 

    // Forcefully attaching socket to the port
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                &opt, sizeof(opt))) 
    { 
        std::cerr << "Unable to set socket." << std::endl; 
        exit(EXIT_FAILURE); 
    } 
    m_address.sin_family = AF_INET; 
    m_address.sin_addr.s_addr = INADDR_ANY; 
    m_address.sin_port = htons(m_port); 

    //Forcefully attaching socket to the port 
    if (bind(m_server_fd, (struct sockaddr *)&m_address, 
                sizeof(m_address)) < 0) 
    { 
        std::cerr << "Unable to bind." << std::endl; 
        exit(EXIT_FAILURE); 
    } 

    if (listen(m_server_fd, 3) < 0) 
    { 
        std::cerr << "Unable to listen." << std::endl; 
        exit(EXIT_FAILURE); 
    } 
}

void Server::start()
{
    std::vector<std::string> splitedOrder;
    char buffer[size_max_request] = {0}; 
    int new_socket;
    int valread; 
    int addrlen = sizeof(m_address); 
    int numberTask = 0;

    while (1)
    {
        //Connection from a new client
        if ((new_socket = accept(m_server_fd, (struct sockaddr *)&m_address, 
                        (socklen_t*)&(addrlen))) < 0) 
        { 
            std::cerr << "Unable accept a new incoming connection." << std::endl; 
            continue;
        } 

        valread = read(new_socket, buffer, size_max_request); 
        
        if (valread == 0)
            continue;

        std::cout << "New client request : " << buffer << std::endl;

        boost::split(splitedOrder, buffer, [](char c){return c == ' ';});

        try 
        {
            //Parse the user request
            auto userRequestTime = userRequestTimeInput(splitedOrder.at(request_time));
            //Discard the request if client request is in the past
            if (userRequestTime < nowFromMidnight())
            {
                std::cout << "User request discard : the request is in the past" << std::endl;
                continue;
            }
            Data d = m_dataSet.at(splitedOrder.at(request_symbol));
            auto it = d.m_data.upper_bound(userRequestTime);

            if (it == std::end(d.m_data))
            {
                std::cout << "User request discard : no market data available for this date." << std::endl;
                continue;
            }

            //Send the new task(s) to the WorkerManager
            if (m_mode == 1)
            {
                m_workerManager.dispatchTask({new_socket, splitedOrder.at(0), ms(it->first).count(), it->second});
                numberTask = 1;
            }
            else
            {
                for (it; it != std::end(d.m_data); it++)
                {
                    m_workerManager.dispatchTask({new_socket, splitedOrder.at(0), ms(it->first).count(), it->second});
                    numberTask += 1;
                }
            }
            std::cout << numberTask << " tasks added." << std::endl;
            numberTask = 0;
            memset(buffer, 0, size_max_request);
        }
        catch (std::exception& e)
        {
            std::cerr << "Unable to interpret user request : " << buffer << std::endl;
            continue;
        }
    }
}
