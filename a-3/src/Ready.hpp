#ifndef ERTS_A3_READY_HPP
#define ERTS_A3_READY_HPP

#include "OperationalState.hpp"
#include "StateOutput.hpp"

class Ready final : public OperationalState {
public:
    static Ready& getInstance()
    {
        static Ready instance;
        return instance;
    }

    void invoke() override
    {
        printState("operationalstate", "Ready");
    }

    Ready(const Ready&) = delete;
    Ready& operator=(const Ready&) = delete;

private:
    Ready() = default;
};

#endif
