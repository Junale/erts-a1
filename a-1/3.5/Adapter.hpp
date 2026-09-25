#ifndef ADAPTER_HPP
#define ADAPTER_HPP

#include <systemc>
#include "Config.hpp"

using namespace sc_core;

SC_MODULE(Adapter)
{

    sc_in<bool> clk;
    sc_in<bool> ready;

    sc_fifo_in<sc_dt::sc_uint<DATA_BITS>> data_in;

    sc_out<bool> valid;

    sc_out<sc_dt::sc_uint<DATA_BITS>> data;
    sc_out<sc_dt::sc_uint<CHANNEL_BITS>> channel;
    sc_out<sc_dt::sc_uint<ERROR_BITS>> error;

    void run()
    {
        while (true)
        {

            valid.write(false);
            wait();

            sc_dt::sc_uint<DATA_BITS> i = data_in.read();

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

    SC_CTOR(Adapter)
    {
        SC_THREAD(run);
        sensitive << clk.pos();
    };
};

#endif
