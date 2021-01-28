#pragma once

#include <string>
#include <chrono>

unsigned int getCurrentTimestamp();
std::chrono::system_clock::duration userRequestTimeInput(const std::string& t);
std::chrono::system_clock::duration dataMarketTimeInput(const std::string& t);
std::chrono::system_clock::duration nowFromMidnight();
