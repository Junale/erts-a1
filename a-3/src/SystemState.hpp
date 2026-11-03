#ifndef ERTS_A3_SYSTEM_STATE_HPP
#define ERTS_A3_SYSTEM_STATE_HPP

class SystemState {
public:
    virtual ~SystemState() = default;
    virtual void invoke() = 0;
};

#endif
