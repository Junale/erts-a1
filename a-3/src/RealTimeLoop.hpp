#ifndef ERTS_A3_REAL_TIME_LOOP_HPP
#define ERTS_A3_REAL_TIME_LOOP_HPP

#include "ChmodeQueue.hpp"
#include "OperationalState.hpp"
#include "StateOutput.hpp"

#include <iostream>
#include <limits>

class RealTimeLoop final : public OperationalState
{
public:
    static RealTimeLoop &getInstance()
    {
        static RealTimeLoop instance;
        return instance;
    }

    void invoke() override
    {
        printState("operationalstate", "RealTimeLoop");

        int choice = 0;
        std::cout << "Choose chmode (1 or 2): ";
        if (!(std::cin >> choice) || (choice != 1 && choice != 2))
        {
            std::cerr << "Invalid chmode. Please choose 1 or 2\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        if (queue_ != nullptr)
        {
            queue_->push(ChmodeEvent(
                choice == 1 ? ChmodeEvent::Type::Chmode1
                            : ChmodeEvent::Type::Chmode2));
        }
    }

    void setQueue(ChmodeQueue *queue)
    {
        queue_ = queue;
    }

    RealTimeLoop(const RealTimeLoop &) = delete;
    RealTimeLoop &operator=(const RealTimeLoop &) = delete;

private:
    RealTimeLoop() = default;
    ChmodeQueue *queue_ = nullptr;
};

#endif
