#pragma once

#include <map>
#include <chrono>

//A Data class instance is created for each symbol.
//It containes all the timers and values for this specific symbol
//The timer value (system_clock::duration) is the elaps between midnight to the time value
class Data
{
public:
    std::map<std::chrono::system_clock::duration, double> m_data;
};
