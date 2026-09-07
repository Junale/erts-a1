#ifndef MODULE_SINGLE_HPP
#define MODULE_SINGLE_HPP

#include <systemc>
using namespace sc_core;

SC_MODULE(ModuleSingle)
{

    sc_dt::sc_uint<4> counter;

    sc_in<bool> clk;

    void noitify()
    {
        while (true)
        {
            counter++;
            std::cout << "Time:"
                      << sc_time_stamp()
                      << "| Counter: "
                      << counter
                      << std::endl;
            wait();
        }
    }

    SC_CTOR(ModuleSingle) : clk("clk")
    {
        counter = 0;
        SC_THREAD(noitify);
        sensitive << clk.pos();
    }
};

#endif