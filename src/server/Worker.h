#pragma once

#include <list>
#include <thread>
#include <chrono>
#include <iostream>
#include <ratio>
#include <mutex>
#include <condition_variable>

#include "Task.h"

//http://www.cplusplus.com/reference/condition_variable/condition_variable/wait/
class Worker
{
public:
    std::list<Task> m_tasks;
    std::mutex mymutex;
    std::condition_variable mycond;
    bool flag = false;

public:
    //Insert a new task in the list of task.
    //The tasks are sorted with the earliest task at the front of the list.
    //so when a new task is inserted at the front,
    //the waiting thread is refresh and update its waiting with the earliest new task
    void insertNewTask(const Task& task);

    //This is where the value are sent to the client when they are matching the task timestamp
    //This function is executed in a separated thread
    void work();
};

