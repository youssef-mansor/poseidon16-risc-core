#include "alu.h"

namespace poseidon16
{

int32_t ALU::execute(Opcode opcode, int32_t a, int32_t b)
{
    switch (opcode)
    {
    case Opcode::ADD:
    case Opcode::ADDI:
        return a + b;

    case Opcode::SUB:
        return a - b;

    case Opcode::AND:
    case Opcode::ANDI:
        return a & b;

    case Opcode::OR:
    case Opcode::ORI:
        return a | b;

    case Opcode::SLT:
    case Opcode::SLTI:
        return (a < b) ? 1 : 0;

    default:
        throw std::invalid_argument(
            "Unsupported opcode passed to ALU"
        );
    }
}

}