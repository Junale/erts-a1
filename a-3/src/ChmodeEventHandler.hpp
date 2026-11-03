#ifndef ERTS_A3_CHMODE_EVENT_HANDLER_HPP
#define ERTS_A3_CHMODE_EVENT_HANDLER_HPP

#include "ChmodeQueue.hpp"

#include <iostream>

class ChmodeEventHandler
{
public:
    explicit ChmodeEventHandler(ChmodeQueue *queue)
        : queue_(queue)
    {
    }

    void handle(const ChmodeEvent &event)
    {
        std::cout << "event: "
                  << (event.getType() == ChmodeEvent::Type::Chmode1
                          ? "chmode1"
                          : "chmode2")
                  << '\n';
    }

    void runThread()
    {
        while (1)
        {
            if (queue_ != nullptr)
            {
                std::optional<ChmodeEvent> event = queue_->pop();
                if (event.has_value())
                {
                    handle(event.value());
                }
            };
        }
    }

private:
    ChmodeQueue *queue_;
};

#endif
