#ifndef ERTS_A3_CONFIGURATION_HPP
#define ERTS_A3_CONFIGURATION_HPP

#include "OperationalState.hpp"
#include "StateOutput.hpp"

class Configuration final : public OperationalState {
public:
    static Configuration& getInstance()
    {
        static Configuration instance;
        return instance;
    }

    void invoke() override
    {
        printState("operationalstate", "Configuration");
    }

    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

private:
    Configuration() = default;
};

#endif
