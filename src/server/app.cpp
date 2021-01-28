#include "Server.h"
#include "Input.h"
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>


int main(int argc, char const *argv[]) 
{ 
    int mode;
    int port = 8080;
    int thread = 1;
    const char* env_p = std::getenv("MODE");
    std::string marketData;

    //Option setup
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
            ("help", "Produce help message")
            ("input", boost::program_options::value<std::string>(), "Input market data file")
            ("port", boost::program_options::value<int>(), "Server port, default 8080")
            ("thread", boost::program_options::value<int>(), "Number of threads, min 1, default 1");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    //Parsing options
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("thread"))
    {
        thread = vm["thread"].as<int>();
        if (thread == 0)
        {
            std::cout << desc << "\n";
            return 1;
        }

    }

    if (vm.count("port")) {
        port = vm["port"].as<int>();
    }

    if (vm.count("input"))
    {
        marketData = vm["input"].as<std::string>();
    }
    else
    {
        std::cout << desc << "\n";
        return 1;
    }

    //Get env variable
    if (env_p and (strcmp(env_p, "1") == 0 or strcmp(env_p, "2") == 0))
    {
        mode = atoi(env_p);
    }
    else
    {
        std::cerr << "Set the environment MODE to 1 or 2" << std::endl;
        return 1;
    }

    std::cout << "App is starting..." << std::endl;

    Server server(Input::readMarketData(marketData), mode, port, thread);
    server.setup();
    server.start();

	return 0; 
} 
