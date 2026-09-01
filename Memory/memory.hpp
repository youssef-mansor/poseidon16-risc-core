#include <iostream>  
#include <cstdint>
#include <array>     
#include <vector>   
#include <stdexcept> // for throwing an error on bad input

using u8  = uint8_t;
using u16 = uint16_t;   
using u32 = uint32_t;  

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;

class MemState {
public:
    //MemState() = default;  

    MemState(const std::vector<uint8_t>& program) { // this will be edited upon itegration of the de-assembler 
        if (program.size() > memory.size()) {
            throw std::out_of_range("Program too large for memory");
        }
        for (size_t i = 0; i < program.size(); ++i) {
            memory[i] = program[i];
        }
    }

    void next_instr() { // since it is byte addressable and instructions is 2 bytes
        pc += 2; 
    } 
    void update_pc(u32 offset) {  // to handle jumps and conditions
        pc += offset; 
    }

    u32 get_pc() const { 
        return pc; 
    }        
    u16 fetch16() const { // big endian 
        u8 high_byte  = memory[pc];
        u8 low_byte   = memory[pc + 1];
        return (static_cast<u16>(high_byte) << 8 | low_byte);
    }
    
private:
    std::array<u8, 64> memory{}; // maximum number of isntructions is 32, 64 bytes 
    u32 pc = 0;
};

//................. Register file is implmented as struct nothing worth to be hidden in a private section so that is why not a class
struct RegFile {

    std::array<i32, 16> regs{};

    i32 read(int idx) const {
        if (idx == 0) return 0;  // r0 is hardwired to zero
        else return regs[idx];   
    }

    void write(int idx, i32 value) {
        if (idx == 0) return;   // guard: writes to r0 is ignored
        regs[idx] = value;
    }
};

struct Data_mem { // big endian 

    std::array<u8, 128> D_mem{}; // size is 128 bytes  u or i ??????

    i16 load(u8 address) const {
        u8 high_byte = D_mem[address];
        u8 low_byte  = D_mem[address + 1];
        return static_cast<i16>((static_cast<u16>(high_byte) << 8) | low_byte);
    }

    void store(u8 address, i16 value) {
        D_mem[address]     = static_cast<u8>(value >> 8); // highest byte
        D_mem[address + 1] = static_cast<u8>(value & 0xFF); // lower byte masking zeros with the highest byte
    }
};
