#include "WorkerManager.h"

WorkerManager::WorkerManager(int numberWorker)
    : m_loadBalancer(0)
{
    for (int i = 0; i < numberWorker; i++)
    {
        std::shared_ptr<Worker> w = std::make_shared<Worker>();
        m_workers.emplace_back(w);
    }
    std::cout << numberWorker << " workers are created." << std::endl;
}

void WorkerManager::start()
{
    for (unsigned int i = 0; i < m_workers.size(); i++)
    {
        std::thread t1(&Worker::work, m_workers.at(i));
        t1.detach();
    }
}

void WorkerManager::dispatchTask(const Task& task)
{
    m_loadBalancer = (m_loadBalancer + 1) % m_workers.size();
    m_workers.at(m_loadBalancer)->insertNewTask(task);
}
