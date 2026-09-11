#ifndef MASTER_HPP
#define MASTER_HPP

#include <systemc>
#include "Config.hpp"

using namespace sc_core;

SC_MODULE(Master)
{

    sc_in<bool> clk;
    sc_in<bool> ready;

    sc_out<bool> valid;

    sc_out<sc_dt::sc_uint<DATA_BITS>> data;
    sc_out<sc_dt::sc_uint<CHANNEL_BITS>> channel;
    sc_out<sc_dt::sc_uint<ERROR_BITS>> error;

    void run()
    {
        valid.write(false);
        wait();

        for (int i = 0; i < 10; i++)
        {
            data.write(i);
            channel.write(0);
            error.write(0);
            valid.write(true);

            wait();

            while (!ready.read())
            {
                wait();
            }

            valid.write(false);
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
