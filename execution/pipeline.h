
#ifndef POSEIDON16_PIPELINE_H
#define POSEIDON16_PIPELINE_H

#include "ISA.h"
#include "alu.h"
#include "memory.hpp"

#include <cstdint>
#include <iostream>
#include <string>

namespace poseidon16
{

class Pipeline
{
public:

    Pipeline(
        MemState& instruction_memory,
        RegFile& registers,
        Data_mem& data_memory
    );

    bool step();

    void run(std::uint32_t max_instructions = 1000);

private:

    MemState& instruction_memory;
    RegFile& registers;
    Data_mem& data_memory;

    Decoder decoder;
    Disassembler disassembler;

    void execute_alu(const DecodedInstruction& instruction);
    void execute_load(const DecodedInstruction& instruction);
    void execute_store(const DecodedInstruction& instruction);
    void execute_branch(const DecodedInstruction& instruction);
    void execute_jal(const DecodedInstruction& instruction);


    //Calculate EA = R[rs1] + SignExt(offset)
    i32 effective_address(const DecodedInstruction& instruction) const;
};

}

#endif // POSEIDON16_PIPELINE_H