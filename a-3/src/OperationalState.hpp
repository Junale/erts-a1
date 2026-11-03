#ifndef ERTS_A3_OPERATIONAL_STATE_HPP
#define ERTS_A3_OPERATIONAL_STATE_HPP

class OperationalState {
public:
    virtual ~OperationalState() = default;
    virtual void invoke() = 0;
};

#endif
