#include "state.h"

namespace poseidon16
{

i32 RegFile::read(int idx) const
{
    if (idx == 0) return 0;  // r0 is hardwired to zero
    return regs[idx];
}

void RegFile::write(int idx, i32 value)
{
    if (idx == 0) return;    // guard: writes to r0 are ignored
    regs[idx] = value;
}

i16 Data_mem::load(u8 address) const
{
    u8 high_byte = D_mem[address];
    u8 low_byte  = D_mem[address + 1];
    return static_cast<i16>((static_cast<u16>(high_byte) << 8) | low_byte);
}

void Data_mem::store(u8 address, i16 value)
{
    D_mem[address]     = static_cast<u8>(value >> 8);   // highest byte
    D_mem[address + 1] = static_cast<u8>(value & 0xFF); // lower byte
}

}