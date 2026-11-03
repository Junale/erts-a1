#ifndef ERTS_A3_POWER_ON_SELF_TEST_HPP
#define ERTS_A3_POWER_ON_SELF_TEST_HPP

#include "StateOutput.hpp"
#include "SystemState.hpp"

class PowerOnSelfTest final : public SystemState {
public:
    static PowerOnSelfTest& getInstance()
    {
        static PowerOnSelfTest instance;
        return instance;
    }

    void invoke() override
    {
        printState("systemstate", "PowerOnSelfTest");
    }

    PowerOnSelfTest(const PowerOnSelfTest&) = delete;
    PowerOnSelfTest& operator=(const PowerOnSelfTest&) = delete;

private:
    PowerOnSelfTest() = default;
};

#endif
