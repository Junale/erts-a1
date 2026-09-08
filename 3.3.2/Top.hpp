#ifndef TOP_HPP
#define TOP_HPP

#include <systemc>
#include "Producer.hpp"
#include "Consumer.hpp"
#include "TCP.hpp"

using namespace sc_core;

SC_MODULE(Top)
{
    sc_clock clock;

    Producer producer;
    Consumer consumer1;
    Consumer consumer2;
    sc_fifo<TCPHeader *> fifo1;
    sc_fifo<TCPHeader *> fifo2;

    void simulate()
    {
        wait(200, SC_NS);
        sc_stop();
    }

    SC_CTOR(Top) : producer("producer"), consumer1("consumer1"), consumer2("consumer2"), fifo1("fifo1", 10), fifo2("fifo2", 10)
    {
        SC_THREAD(simulate);

        producer.fifo_out(fifo1);
        producer.fifo_out(fifo2);

        consumer1.fifo_in(fifo1);
        consumer2.fifo_in(fifo2);
    }
};

#endif