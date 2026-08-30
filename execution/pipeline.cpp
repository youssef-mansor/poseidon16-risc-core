#include "pipeline.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace poseidon16
{

Pipeline::Pipeline(
    MemState& instruction_memory,
    RegFile& registers,
    Data_mem& data_memory
)
    : instruction_memory(instruction_memory),
      registers(registers),
      data_memory(data_memory)
{
}

// execute the complete cycle of a single instruction, return true if the instruction was executed successfully
bool Pipeline::step()
{
    //fetch
    std::uint16_t raw_instruction = instruction_memory.fetch16();
    std::uint32_t current_pc = instruction_memory.get_pc();

    //decode and disassemble
    DecodedInstruction instruction = decoder.decode(raw_instruction);
    std::string assembly = disassembler.disassemble(instruction);

    std::cout
        << "PC=0x"
        << std::hex
        << current_pc
        << "  "
        << assembly
        << std::dec
        << '\n';

    //execute
    switch (instruction.opcode)
    {
    case Opcode::ADD:
    case Opcode::SUB:
    case Opcode::AND:
    case Opcode::OR:
    case Opcode::SLT:
    case Opcode::ADDI:
    case Opcode::ANDI:
    case Opcode::ORI:
    case Opcode::SLTI:
        execute_alu(instruction);
        break;

    case Opcode::LW:
        execute_load(instruction);
        break;

    case Opcode::SW:
        execute_store(instruction);
        break;

    case Opcode::BEQ:
        execute_branch(instruction);
        return true;

    case Opcode::JAL:
        execute_jal(instruction);
        return true;

    default:
        throw std::runtime_error("Unknown or unsupported opcode");
    }

    instruction_memory.next_instr();

    return true;
}


void Pipeline::execute_alu(const DecodedInstruction& instruction)
{
    i32 a = registers.read(instruction.rs1);
    i32 b;

    if (instruction.format == Format::R)
    {
        b = registers.read(instruction.rs2);
    }
    else
    {
        b = instruction.immediate;
    }

    i32 result = ALU::execute(instruction.opcode, a, b);

    registers.write(instruction.rd, result);
}

i32 Pipeline::effective_address(const DecodedInstruction& instruction) const
{
    i32 base = registers.read(instruction.rs1);
    i32 offset = instruction.immediate;

    return ALU::execute(Opcode::ADD, base, offset);
}

void Pipeline::execute_load(const DecodedInstruction& instruction)
{
    i32 address = effective_address(instruction);

    if (address < 0 || address + WORD_BYTES > 128)
    {
        throw std::out_of_range("LW address outside data memory");
    }

    i32 value = data_memory.load(static_cast<u8>(address));
    registers.write(instruction.rd, value);
}

void Pipeline::execute_store(const DecodedInstruction& instruction)
{
    i32 address = effective_address(instruction);

    if (address < 0 || address + WORD_BYTES > 128)
    {
        throw std::out_of_range("SW address outside data memory");
    }

    i32 value = registers.read(instruction.rs2);
    data_memory.store(static_cast<u8>(address), value);
}

void Pipeline::execute_branch(const DecodedInstruction& instruction)
{
    i32 rs1 = registers.read(instruction.rs1);
    i32 rs2 = registers.read(instruction.rs2);

    if (rs1 == rs2)
    {
        instruction_memory.update_pc(instruction.immediate);
    }
    else
    {
        instruction_memory.next_instr();
    }
}

void Pipeline::execute_jal(const DecodedInstruction& instruction)
{
    std::uint32_t current_pc = instruction_memory.get_pc();

    i32 return_address = static_cast<i32>(current_pc + INSTRUCTION_BYTES);

    registers.write(instruction.rd, return_address);
    instruction_memory.update_pc(instruction.immediate);
}

void Pipeline::run(std::uint32_t max_instructions)
{
    for (std::uint32_t i = 0; i < max_instructions; ++i)
    {
        step();
    }
}

}