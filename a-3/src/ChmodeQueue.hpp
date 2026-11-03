#ifndef ERTS_A3_CHMODE_QUEUE_HPP
#define ERTS_A3_CHMODE_QUEUE_HPP

#include "ChmodeEvent.hpp"

#include <optional>
#include <queue>

class ChmodeQueue {
public:
    void push(const ChmodeEvent& event)
    {
        queue_.push(event);
    }

    std::optional<ChmodeEvent> pop()
    {
        if (queue_.empty()) {
            return std::nullopt;
        }

        ChmodeEvent event = queue_.front();
        queue_.pop();
        return event;
    }

private:
    std::queue<ChmodeEvent> queue_;
};

#endif
