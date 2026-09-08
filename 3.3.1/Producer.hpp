#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <systemc>
#include "TCP.hpp"

using namespace sc_core;

SC_MODULE(Producer)
{
    sc_fifo_out<TCPHeader *> fifo_out;

    void run()
    {
        while (true)
        {
            TCPHeader *val = new TCPHeader();
            val->SourcePort = 1234;
            val->DestinationPort = 5678;
            val->SequenceNumber = 1;
            val->Acknowledge = 0;
            val->StatusBits = 0;
            val->WindowSize = 0;
            val->Checksum = 0;
            val->UrgentPointer = 0;

            std::string data = "Hello, World!";
            std::copy(data.begin(), data.end(), val->Data);
            fifo_out.write(val);
            // Random delay between 2 and 10 ns
            int random_delay = rand() % 10 + 2;
            wait(random_delay, SC_NS);
        }
    }

    SC_CTOR(Producer) : fifo_out("fifo_out")
    {
        SC_THREAD(run);
    }
};

#endif