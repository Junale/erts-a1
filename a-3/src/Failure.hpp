#ifndef ERTS_A3_FAILURE_HPP
#define ERTS_A3_FAILURE_HPP

#include "StateOutput.hpp"
#include "SystemState.hpp"

class Failure final : public SystemState {
public:
    static Failure& getInstance()
    {
        static Failure instance;
        return instance;
    }

    void invoke() override
    {
        printState("systemstate", "Failure");
    }

    Failure(const Failure&) = delete;
    Failure& operator=(const Failure&) = delete;

private:
    Failure() = default;
};

#endif
