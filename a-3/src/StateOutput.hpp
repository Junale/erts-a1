#ifndef ERTS_A3_STATE_OUTPUT_HPP
#define ERTS_A3_STATE_OUTPUT_HPP

#include <iostream>

inline void printState(const char* type, const char* name)
{
    std::cout << '{' << type << ", " << name << '}' << '\n';
}

#endif
