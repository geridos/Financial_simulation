#include <boost/algorithm/string.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <exception>

#include "Input.h"
#include "TimeToolKit.h"

constexpr int dm_date = 0;
constexpr int dm_symbol = 1;
constexpr int dm_value = 2;

std::map<std::string, Data> Input::readMarketData(const std::string& file)
{
    std::map<std::string, Data> dataSet;
    std::vector<std::string> lineTokens;
    std::map<std::string, Data>::iterator it;
    std::ifstream dataMarket(file);
    std::string line;

    if (dataMarket.is_open())
    {
        std::cout << "Start reading data market..." << std::endl;
        while (getline(dataMarket, line))
        {
            if (line[0] == '#')
                continue;

            boost::split(lineTokens, line, [](char c){return c == ',';});

            try
            {
                auto duration = dataMarketTimeInput(lineTokens.at(dm_date));
                auto value = atof(lineTokens.at(dm_value).c_str());
                if ((it = dataSet.find(lineTokens.at(dm_symbol))) != std::end(dataSet))
                {
                    it->second.m_data[duration] = value;
                }
                else {
                    Data d;
                    d.m_data[duration] = value;
                    dataSet[lineTokens[dm_symbol]] = d;
                }
            }
            catch (std::exception& e)
            {
                std::cerr << "Error while parsing the line : "
                    << line << ". Reason : " << e.what() << std::endl;
            }
        }
        dataMarket.close();
        std::cout << "Done." << std::endl;
    }
    else {
        std::cerr << "Unable to open : " << file << std::endl;
        exit(EXIT_FAILURE);
    }
    return dataSet;
}
