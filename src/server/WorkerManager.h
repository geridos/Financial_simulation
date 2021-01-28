#pragma once

#include "Worker.h"
#include "vector"

class WorkerManager
{

public:
    //Create the worker
    WorkerManager(int numberWorker);

    //Launch the threads
    void start();

    //dispatch tasks to the workers
    void dispatchTask(const Task& task);

private:
    int m_loadBalancer;
    std::vector<std::shared_ptr<Worker>> m_workers;
};
