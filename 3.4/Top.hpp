#ifndef TOP_HPP
#define TOP_HPP

#include <systemc>
#include "Master.hpp"
#include "Slave.hpp"

#include "Config.hpp"

using namespace sc_core;

SC_MODULE(Top)
{
    sc_clock clock;
    Master master;
    Slave slave;

    sc_signal<bool> valid_signal;
    sc_signal<bool> ready_signal;

    sc_signal<sc_dt::sc_uint<DATA_BITS>> data_signal;
    sc_signal<sc_dt::sc_uint<CHANNEL_BITS>> channel_signal;
    sc_signal<sc_dt::sc_uint<ERROR_BITS>> error_signal;

    sc_trace_file *tf;

    void simulate()
    {
        wait(50 * CLK_PERIODE, SC_NS);
        sc_stop();
    }

    SC_CTOR(Top) : clock("clock", CLK_PERIODE, SC_NS), master("master"), slave("slave")
    {
        master.clk(clock);
        master.ready(ready_signal);
        master.valid(valid_signal);
        master.data(data_signal);
        master.channel(channel_signal);
        master.error(error_signal);

        slave.clk(clock);
        slave.valid(valid_signal);
        slave.data(data_signal);
        slave.channel(channel_signal);
        slave.error(error_signal);
        slave.ready(ready_signal);

        tf = sc_create_vcd_trace_file("WaveForm");
        tf->set_time_unit(1, SC_NS);
        sc_trace(tf, clock, "clock");
        sc_trace(tf, valid_signal, "valid");
        sc_trace(tf, data_signal, "data");
        sc_trace(tf, channel_signal, "channel");
        sc_trace(tf, error_signal, "error");
        sc_trace(tf, ready_signal, "ready");

        SC_THREAD(simulate);
    }
};

#endif