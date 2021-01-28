#pragma once

#include <string>

//A Task contains the socket file descriptor of the client and one set of data market
class Task
{
public:
    Task(int client, const std::string& symbol, long int time, double val);

    int getFdClient() const;
    std::string getSymbol() const;
    long int getShreduledTimer() const;
    double getValue() const;

private:
    int m_fd_client;
    std::string m_symbol;
    long int m_time;
    double m_val;
};
