#ifndef POSEIDON16_ALU_H
#define POSEIDON16_ALU_H

#include "ISA.h"
#include <cstdint>
#include <stdexcept>

namespace poseidon16
{

class ALU
{
public:
    static int32_t execute(Opcode opcode, int32_t a, int32_t b);
};

}

#endif // POSEIDON16_ALU_H