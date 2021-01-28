#include "Task.h"

Task::Task(int client, const std::string& symbol, long int time, double val)
    : m_fd_client(client),
    m_symbol(symbol),
    m_time(time),
    m_val(val)
{ }

int Task::getFdClient() const
{
    return m_fd_client;
}

std::string Task::getSymbol() const
{
    return m_symbol;
}

long int Task::getShreduledTimer() const
{
    return m_time;
}

double Task::getValue() const
{
    return m_val;
}
