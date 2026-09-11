#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <systemc>
#include "Config.hpp"

using namespace sc_core;

SC_MODULE(Slave)
{

    sc_in<bool> clk;
    sc_in<bool> valid;

    sc_in<sc_dt::sc_uint<DATA_BITS>> data;
    sc_in<sc_dt::sc_uint<CHANNEL_BITS>> channel;
    sc_in<sc_dt::sc_uint<ERROR_BITS>> error;

    sc_out<bool> ready;

    void run()
    {
        ready.write(false);
        wait();

        for (int i = 0; i < 10; i++)
        {
            while (!valid.read())
            {
                wait();
            }
            ready.write(true);
            wait();
            ready.write(false);
            wait();
        }
    }

    SC_CTOR(Slave)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    };
};

#endif
