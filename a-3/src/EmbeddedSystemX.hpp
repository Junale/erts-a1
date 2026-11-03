#ifndef ERTS_A3_EMBEDDED_SYSTEM_X_HPP
#define ERTS_A3_EMBEDDED_SYSTEM_X_HPP

#include "Failure.hpp"
#include "Initializing.hpp"
#include "Operational.hpp"
#include "PowerOnSelfTest.hpp"

#include <iostream>

class EmbeddedSystemX
{
public:
    void setState(SystemState *state)
    {
        // check from state
        const bool fromPowerOnSelfTest =
            dynamic_cast<PowerOnSelfTest *>(state_) != nullptr;
        const bool fromFailure = dynamic_cast<Failure *>(state_) != nullptr;
        const bool fromInitializing =
            dynamic_cast<Initializing *>(state_) != nullptr;
        const bool fromOperational =
            dynamic_cast<Operational *>(state_) != nullptr;

        // check to state
        const bool toPowerOnSelfTest =
            dynamic_cast<PowerOnSelfTest *>(state) != nullptr;
        const bool toFailure = dynamic_cast<Failure *>(state) != nullptr;
        const bool toInitializing =
            dynamic_cast<Initializing *>(state) != nullptr;
        const bool toOperational =
            dynamic_cast<Operational *>(state) != nullptr;

        // Validate state transition
        const bool validTransition =
            (state_ == nullptr && toPowerOnSelfTest) ||
            (fromPowerOnSelfTest && (toFailure || toInitializing)) ||
            (fromFailure && toPowerOnSelfTest) ||
            (fromInitializing && toOperational) ||
            (fromOperational && toPowerOnSelfTest);

        if (!validTransition)
        {
            std::cerr << "Invalid system state transition\n";
            return;
        }

        // Set State
        state_ = state;

        // Invoke the state
        state_->invoke();
    }

private:
    SystemState *state_ = nullptr;
};

#endif
