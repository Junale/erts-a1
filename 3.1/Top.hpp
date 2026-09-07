#ifndef TOP_HPP
#define TOP_HPP

#include <systemc>
#include "ModuleSingle.hpp"

SC_MODULE(Top)
{
    sc_core::sc_clock clock;
    ModuleSingle ms;

    void simulate()
    {
        wait(200, SC_NS);
        sc_stop();
    }

    SC_CTOR(Top) : clock("clock", 2, sc_core::SC_NS), ms("ms")
    {
        ms.clk(clock);
        SC_THREAD(simulate);
    }
};

#endif