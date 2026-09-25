#ifndef MASTER_HPP
#define MASTER_HPP

#include <systemc>
#include "Config.hpp"

using namespace sc_core;

SC_MODULE(Master)
{
    sc_in<bool> clk;

    sc_fifo_out<sc_dt::sc_uint<DATA_BITS>> data_out;

    void run()
    {
        wait();

        for (int i = 0; i < 10; ++i)
        {
            data_out.write(i);
            wait();
        }
    }

    SC_CTOR(Master)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    };
};

#endif