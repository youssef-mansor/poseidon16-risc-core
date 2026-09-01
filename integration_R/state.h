#ifndef POSEIDON16_STATE_H
#define POSEIDON16_STATE_H

#include "ISA.h"
#include <array>

namespace poseidon16
{

struct RegFile
{
public:
    i32 read(int idx) const;
    void write(int idx, i32 value);

private:
    std::array<i32, NUM_REGISTERS> regs{};
};


struct Data_mem
{
public:
    i16 load(u8 address) const;
    void store(u8 address, i16 value);

private:
    std::array<u8, 256> D_mem{};
};

}

#endif