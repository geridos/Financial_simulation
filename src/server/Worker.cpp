#include <strstream>
#include <sys/socket.h> 
#include <iomanip>

#include "Worker.h"
#include "TimeToolKit.h"

#define ms(X) std::chrono::duration_cast<std::chrono::microseconds>(X)

void Worker::insertNewTask(const Task& task)
{
    for (auto t = std::begin(m_tasks); t != std::end(m_tasks); t++)
    {
        if (task.getShreduledTimer() <= t->getShreduledTimer())
        {
            m_tasks.insert(t, task);
            if (t == std::next(std::begin(m_tasks)))
            {
                //notify the thread that a earliest task is added
                std::lock_guard<std::mutex> lock(mymutex);
                flag = true;
                mycond.notify_one();
            }
            return;
        }
    }
    m_tasks.push_back(task);
}

void Worker::work()
{
    std::unique_lock<std::mutex> lock(mymutex);
    std::string toSend;

    while (true)
    {
        if (m_tasks.empty())
            continue;

        Task& task = m_tasks.front();


        auto timer = task.getShreduledTimer() - ms(nowFromMidnight()).count();

        if (timer < 0)
        {
            std::cerr << "The app is struggling to keep up the pace. Re start the app with more threads." << std::endl;
        }
        else
        {
            mycond.wait_for(lock,
                    std::chrono::microseconds(timer),
                    [this]() { return flag; } );
        }        

        if (flag)
        {
            flag = false;
            continue;
        }

        std::ostrstream v;
        v << std::fixed << std::setprecision(3) << task.getValue();

        toSend = task.getSymbol() + " @ " + std::to_string(getCurrentTimestamp()) + " = " + v.str();

        std::cout << toSend << std::endl;

        send(task.getFdClient(), toSend.data(), toSend.size(), 0); 
        m_tasks.pop_front();
    }
}
