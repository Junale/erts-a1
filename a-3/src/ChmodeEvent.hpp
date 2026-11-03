#ifndef ERTS_A3_CHMODE_EVENT_HPP
#define ERTS_A3_CHMODE_EVENT_HPP

class ChmodeEvent {
public:
    enum class Type {
        Chmode1,
        Chmode2
    };

    explicit ChmodeEvent(Type type)
        : type_(type)
    {
    }

    Type getType() const
    {
        return type_;
    }

private:
    Type type_;
};

#endif
