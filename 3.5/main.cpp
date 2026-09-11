#ifndef MAIN_CPP
#define MAIN_CPP

#include <systemc>
#include "Top.hpp"

using namespace sc_core;

int sc_main(int argc, char *argv[])
{
    Top top("top");
    sc_start();
    return 0;
}

#endif
