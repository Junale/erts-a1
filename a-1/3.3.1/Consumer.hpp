#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include <systemc>
#include "TCP.hpp"

using namespace sc_core;

SC_MODULE(Consumer)
{
    sc_fifo_in<TCPHeader *> fifo_in;

    void run()
    {
        while (true)
        {
            TCPHeader *val = fifo_in.read();
            std::cout << "Read " << val->Data << " at " << sc_time_stamp() << std::endl;
            delete val;
        }
    }

    SC_CTOR(Consumer) : fifo_in("fifo_in")
    {
        SC_THREAD(run);
    }
};

#endif
