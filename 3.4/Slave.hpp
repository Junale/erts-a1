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

    sc_trace_file *tf;

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

        tf = sc_create_vcd_trace_file("WaveForm");
        tf->set_time_unit(1, SC_NS);
        sc_trace(tf, clk, "clock");
        sc_trace(tf, valid, "valid");
        sc_trace(tf, data, "data");
        sc_trace(tf, channel, "channel");
        sc_trace(tf, error, "error");
        sc_trace(tf, ready, "ready");

        SC_THREAD(run);
        sensitive << clk.pos();
    };

    ~Slave()
    {
        sc_close_vcd_trace_file(tf);
    }
};

#endif
