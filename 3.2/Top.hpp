#ifndef TOP_HPP
#define TOP_HPP

#include <systemc>
#include "ModuleDouble.hpp"

using namespace sc_core;

SC_MODULE(Top)
{
    ModuleDouble md;

    void simulate()
    {
        wait(200, SC_MS);
        sc_stop();
    }

    SC_CTOR(Top) : md("md")
    {
        SC_THREAD(simulate);
    }
};

#endif