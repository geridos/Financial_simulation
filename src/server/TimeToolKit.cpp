#include "TimeToolKit.h"

#include <boost/algorithm/string.hpp>
#include "vector"
#include "iostream"

constexpr char TimeRequestFormat[] = "%H:%M";
constexpr char TimeMarketFormat[] = "%H:%M:%S";


static std::chrono::time_point<std::chrono::system_clock> nowTimePoint()
{
    return std::chrono::system_clock::now();
}

static std::time_t getTroncatedTime()
{
    return std::chrono::system_clock::to_time_t(nowTimePoint());
}

//The time of last midnight
static std::chrono::system_clock::time_point midnightTimePoint()
{
    //midnight
    std::time_t time = getTroncatedTime();
    std::tm *midnight = std::localtime(&time);
    midnight->tm_hour = 0; 
    midnight->tm_min = 0;
    midnight->tm_sec = 0;
    return std::chrono::system_clock::from_time_t(std::mktime(midnight));
}

std::chrono::system_clock::duration nowFromMidnight()
{
    std::time_t timeNow = getTroncatedTime();
    std::tm *tm = std::localtime(&timeNow);
    auto n = std::chrono::system_clock::from_time_t(std::mktime(tm));
    return n - midnightTimePoint();
}

//The time elaps between midnight and the user request
std::chrono::system_clock::duration userRequestTimeInput(const std::string& t)
{
    std::time_t timeNow = getTroncatedTime();
    std::tm *tm = std::localtime(&timeNow);
    if (not strptime(t.c_str(), TimeRequestFormat, tm))
    {
        throw "Unable to parse " + t;
    }
    tm->tm_sec = 0;
    auto nxt = std::chrono::system_clock::from_time_t(std::mktime(tm));

    return nxt - midnightTimePoint();
}

//The time elaps between midnight and a data market entry
std::chrono::system_clock::duration dataMarketTimeInput(const std::string& t)
{
    std::vector<std::string> seconds_micro;
    boost::split(seconds_micro, t, [](char c){return c == '.';});

    std::time_t timeNow = getTroncatedTime();
    std::tm *tm = std::localtime(&timeNow);

    if (not strptime(t.c_str(), TimeMarketFormat, tm))
    {
        throw "Unable to parse " + t;
    }

    auto nxt = std::chrono::system_clock::from_time_t(std::mktime(tm));

    return nxt - midnightTimePoint() + std::chrono::microseconds(atoi(seconds_micro[1].c_str()));
}

//Timestamp sent to client
unsigned int getCurrentTimestamp()
{
    std::time_t result = std::time(nullptr);
    std::asctime(std::localtime(&result));
    return result;
}
