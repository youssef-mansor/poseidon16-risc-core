//#include "rom.h"
#include "ISA.h"
#include <stdexcept>

namespace poseidon16
{

ROM::ROM(const std::vector<uint8_t>& program) // this will be edited upon integration of the de-assembler
{
    if (program.size() > memory.size())
    {
        throw std::out_of_range("Program too large for memory");
    }
    for (size_t i = 0; i < program.size(); ++i)
    {
        memory[i] = program[i];
    }
}

void ROM::next_instr()
{
    pc += 2; // since it is byte addressable and instructions is 2 bytes
}

void ROM::update_pc(u32 offset)
{
    pc += offset; // to handle jumps and conditions
}

u32 ROM::get_pc() const
{
    return pc;
}

u16 ROM::fetch16() const // big endian
{
    u8 high_byte = memory[pc];
    u8 low_byte  = memory[pc + 1];
    return (static_cast<u16>(high_byte) << 8 | low_byte);
}

}