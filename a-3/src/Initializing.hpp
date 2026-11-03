#ifndef ERTS_A3_INITIALIZING_HPP
#define ERTS_A3_INITIALIZING_HPP

#include "StateOutput.hpp"
#include "SystemState.hpp"

class Initializing final : public SystemState {
public:
    static Initializing& getInstance()
    {
        static Initializing instance;
        return instance;
    }

    void invoke() override
    {
        printState("systemstate", "Initializing");
    }

    Initializing(const Initializing&) = delete;
    Initializing& operator=(const Initializing&) = delete;

private:
    Initializing() = default;
};

#endif
