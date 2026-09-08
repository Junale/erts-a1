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
    Consumer consumer;
    sc_fifo<TCPHeader *> fifo;

    void simulate()
    {
        wait(2000, SC_NS);
        sc_stop();
    }

    SC_CTOR(Top) : producer("producer"), consumer("consumer"), fifo("fifo", 10)
    {
        SC_THREAD(simulate);

        producer.fifo_out(fifo);
        consumer.fifo_in(fifo);
    }
};

#endif