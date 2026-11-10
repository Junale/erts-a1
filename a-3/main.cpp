#include "ChmodeEventHandler.hpp"
#include "ChmodeQueue.hpp"
#include "EmbeddedSystemX.hpp"
#include "Operational.hpp"
#include "RealTimeLoop.hpp"
#include "PowerOnSelfTest.hpp"
#include "Failure.hpp"
#include "Initializing.hpp"

#include <iostream>
#include <thread>

int main()
{
    EmbeddedSystemX embeddedSystemX;
    ChmodeQueue eventQueue;
    ChmodeEventHandler eventHandler(&eventQueue);
    RealTimeLoop &realTimeLoop = RealTimeLoop::getInstance();
    realTimeLoop.setQueue(&eventQueue);
    Operational &operational = Operational::getInstance();
    std::thread eventHandlerThread(&ChmodeEventHandler::runThread,
                                   &eventHandler);
    embeddedSystemX.setState(&PowerOnSelfTest::getInstance());
    operational.setState(&Ready::getInstance());

    bool running = true;

    while (running)
    {
        std::cout << "\nSelect an action:\n"
                  << "0: Exit\n"
                  << "1: Restart\n"
                  << "2: SelftestFailed\n"
                  << "3: SelftestOk\n"
                  << "4: Initialized\n"
                  << "5: Configure\n"
                  << "6: ConfigurationEnded\n"
                  << "7: Stop\n"
                  << "8: Start/run\n"
                  << "Choice: ";

        int choice = -1;
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input\n";
            continue;
        }

        switch (choice)
        {
        case 0:
            running = false;
            break;
        case 1:
            embeddedSystemX.setState(&PowerOnSelfTest::getInstance());
            break;
        case 2:
            embeddedSystemX.setState(&Failure::getInstance());
            break;
        case 3:
            embeddedSystemX.setState(&Initializing::getInstance());
            break;
        case 4:
            embeddedSystemX.setState(&Operational::getInstance());
            break;
        case 5:
            operational.setState(&Configuration::getInstance());
            break;
        case 6:
            operational.setState(&Ready::getInstance());
            break;
        case 7:
            operational.setState(&Ready::getInstance());
            break;
        case 8:
            operational.setState(&realTimeLoop);
            break;
        default:
            std::cout << "Invalid choice\n";
            break;
        }
    }

    eventHandlerThread.join();

    return 0;
}