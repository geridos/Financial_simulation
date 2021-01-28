#pragma once

#include "Data.h"

class Input
{
public:
    //Parse the market data file
    //Each entries of the map are a market symbol and its Data
    static std::map<std::string, Data> readMarketData(const std::string& file);
};
