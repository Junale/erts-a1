#ifndef ERTS_A3_OPERATIONAL_HPP
#define ERTS_A3_OPERATIONAL_HPP

#include "Configuration.hpp"
#include "OperationalState.hpp"
#include "Ready.hpp"
#include "RealTimeLoop.hpp"
#include "StateOutput.hpp"
#include "SystemState.hpp"

#include <iostream>

class Operational final : public SystemState
{
public:
    static Operational &getInstance()
    {
        static Operational instance;
        return instance;
    }

    void invoke() override
    {
        printState("systemstate", "Operational");
    }

    void setState(OperationalState *state)
    {
        // check from state
        const bool fromReady = dynamic_cast<Ready *>(state_) != nullptr;
        const bool fromConfiguration =
            dynamic_cast<Configuration *>(state_) != nullptr;
        const bool fromRealTimeLoop =
            dynamic_cast<RealTimeLoop *>(state_) != nullptr;

        // check to state
        const bool toReady = dynamic_cast<Ready *>(state) != nullptr;
        const bool toConfiguration =
            dynamic_cast<Configuration *>(state) != nullptr;
        const bool toRealTimeLoop =
            dynamic_cast<RealTimeLoop *>(state) != nullptr;

        // Validate state transition
        const bool validTransition =
            (state_ == nullptr && toReady) ||
            (fromReady && (toConfiguration || toRealTimeLoop)) ||
            (fromRealTimeLoop && toReady) ||
            (fromConfiguration && toReady);

        if (!validTransition)
        {
            std::cerr << "Invalid operational state transition\n";
            return;
        }

        // Set State
        state_ = state;

        // Invoke the state
        state_->invoke();
    }

    Operational(const Operational &) = delete;
    Operational &operator=(const Operational &) = delete;

private:
    Operational() = default;
    OperationalState *state_ = nullptr;
};

#endif
