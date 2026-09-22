#ifndef MODULE_DOUBLE_HPP
#define MODULE_DOUBLE_HPP

#include <systemc>
using namespace sc_core;

SC_MODULE(ModuleDouble)
{
    sc_event eventA, eventB, eventAack, eventBack;
    bool waitForA = true;
    bool firstCall = true;

    void threadA()
    {
        while (true)
        {
            eventA.notify();
            wait(3, SC_MS, eventAack);
        }
    }

    void threadB()
    {
        while (true)
        {
            eventB.notify();
            wait(2, SC_MS, eventBack);
        }
    }

    void methodA()
    {
        if (firstCall)
        {
            firstCall = false;
            next_trigger(eventA);
            return;
        }
        if (waitForA)
        {
            std::cout << "Time: " << sc_time_stamp() << " | Event: A" << std::endl;
            eventAack.notify();
            waitForA = false;
            next_trigger(eventB);
        }
        else
        {
            std::cout << "Time: " << sc_time_stamp() << " | Event: B" << std::endl;
            eventBack.notify();
            waitForA = true;
            next_trigger(eventA);
        }
    }

    SC_CTOR(ModuleDouble)
    {
        SC_THREAD(threadA);
        SC_THREAD(threadB);
        SC_METHOD(methodA);
    }
};

#endif