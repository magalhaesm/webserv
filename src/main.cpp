#include <cstdlib>
#include <iostream>

#include "Logger.hpp"
#include "Server.hpp"
#include "ConfigParser.hpp"
#include "EventListener.hpp"
#include "InternalErrorException.hpp"

void show_banner();

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " [CONFIGURATION FILE]" << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        ConfigParser parser(argv[1]);
        ConfigSpecList specs = parser.getSpecs();
        EventListener listener;
        show_banner();
        for (size_t idx = 0; idx < specs.size(); ++idx)
        {
            listener.subscribe(new Server(specs[idx]));
        }
        listener.start();
    }
    catch (const InternalErrorException& e)
    {
        Logger::log(e.what());
        return e.errnum();
    }
    catch (const std::exception& e)
    {
        Logger::log(e.what());
    }
}

void show_banner()
{
    const char* asciiArt = "\
,--.   ,--.       ,--.    ,---.\n\
|  |   |  | ,---. |  |-. '   .-'  ,---. ,--.--.,--.  ,--.\n\
|  |.'.|  || .-. :| .-. '`.  `-. | .-. :|  .--' \\  `'  /\n\
|   ,'.   |\\   --.| `-' |.-'    |\\   --.|  |     \\    /\n\
'--'   '--' `----' `---' `-----'  `----'`--'      `--'\n";
    std::cout << asciiArt << '\n';
}
