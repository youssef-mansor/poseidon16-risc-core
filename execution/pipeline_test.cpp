#include "pipeline.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace poseidon16;

std::uint16_t encode_r(Opcode opcode, int rd, int rs1, int rs2)
{
    return (static_cast<std::uint16_t>(opcode) << 12) |
           (rd << 8) |
           (rs1 << 4) |
           rs2;
}

std::uint16_t encode_i(Opcode opcode, int rd, int rs1, int imm)
{
    return (static_cast<std::uint16_t>(opcode) << 12) |
           (rd << 8) |
           (rs1 << 4) |
           (imm & 0xF);
}

std::uint16_t encode_beq(int rs1, int rs2, int offset)
{
    int encoded_offset = offset / CONTROL_OFFSET_SCALE;

    return (static_cast<std::uint16_t>(Opcode::BEQ) << 12) |
           (rs1 << 8) |
           (rs2 << 4) |
           (encoded_offset & 0xF);
}

std::uint16_t encode_jal(int rd, int offset)
{
    int encoded_offset = offset / CONTROL_OFFSET_SCALE;

    return (static_cast<std::uint16_t>(Opcode::JAL) << 12) |
           (rd << 8) |
           (encoded_offset & 0xFF);
}

void append_instruction(std::vector<std::uint8_t>& program, std::uint16_t instruction)
{
    program.push_back(static_cast<std::uint8_t>(instruction >> 8));
    program.push_back(static_cast<std::uint8_t>(instruction & 0xFF));
}

void test_alu_instructions()
{
    std::cout << "\nTesting ALU instructions...\n";

    std::vector<std::uint8_t> program;

    append_instruction(
        program,
        encode_r(Opcode::ADD, 3, 1, 2)
    );

    append_instruction(
        program,
        encode_r(Opcode::SUB, 4, 1, 2)
    );

    append_instruction(
        program,
        encode_r(Opcode::AND, 5, 1, 2)
    );

    append_instruction(
        program,
        encode_r(Opcode::OR, 6, 1, 2)
    );

    append_instruction(
        program,
        encode_r(Opcode::SLT, 7, 1, 2)
    );

    MemState instruction_memory(program);
    RegFile registers;
    Data_mem data_memory;

    registers.write(1, 10);
    registers.write(2, 3);

    Pipeline pipeline(
        instruction_memory,
        registers,
        data_memory
    );

    pipeline.step();
    assert(registers.read(3) == 13);
    assert(instruction_memory.get_pc() == 2);

    pipeline.step();
    assert(registers.read(4) == 7);
    assert(instruction_memory.get_pc() == 4);

    pipeline.step();
    assert(registers.read(5) == (10 & 3));
    assert(instruction_memory.get_pc() == 6);

    pipeline.step();
    assert(registers.read(6) == (10 | 3));
    assert(instruction_memory.get_pc() == 8);

    pipeline.step();
    assert(registers.read(7) == 0);
    assert(instruction_memory.get_pc() == 10);

    std::cout << "ALU instructions passed.\n";
}

void test_immediate_instructions()
{
    std::cout << "\nTesting immediate instructions...\n";

    std::vector<std::uint8_t> program;

    append_instruction(
        program,
        encode_i(Opcode::ADDI, 2, 1, 5)
    );

    append_instruction(
        program,
        encode_i(Opcode::ANDI, 3, 1, 3)
    );

    append_instruction(
        program,
        encode_i(Opcode::ORI, 4, 1, 3)
    );

    append_instruction(
        program,
        encode_i(Opcode::SLTI, 5, 1, 7)
    );

    MemState instruction_memory(program);
    RegFile registers;
    Data_mem data_memory;

    registers.write(1, 10);

    Pipeline pipeline(
        instruction_memory,
        registers,
        data_memory
    );

    pipeline.step();
    assert(registers.read(2) == 15);
    assert(instruction_memory.get_pc() == 2);

    pipeline.step();
    assert(registers.read(3) == (10 & 3));
    assert(instruction_memory.get_pc() == 4);

    pipeline.step();
    assert(registers.read(4) == (10 | 3));
    assert(instruction_memory.get_pc() == 6);

    pipeline.step();
    assert(registers.read(5) == 0);
    assert(instruction_memory.get_pc() == 8);

    std::cout << "Immediate instructions passed.\n";
}

void test_r0()
{
    std::cout << "\nTesting R0...\n";

    std::vector<std::uint8_t> program;

    append_instruction(
        program,
        encode_r(Opcode::ADD, 0, 1, 2)
    );

    MemState instruction_memory(program);
    RegFile registers;
    Data_mem data_memory;

    registers.write(1, 10);
    registers.write(2, 20);

    Pipeline pipeline(
        instruction_memory,
        registers,
        data_memory
    );

    pipeline.step();

    assert(registers.read(0) == 0);

    std::cout << "R0 test passed.\n";
}

void test_beq_taken()
{
    std::cout << "\nTesting BEQ taken...\n";

    std::vector<std::uint8_t> program;

    // PC = 0: BEQ R1, R2, +4
    append_instruction(
        program,
        encode_beq(1, 2, 4)
    );

    // PC = 2: This instruction should be skipped.
    append_instruction(
        program,
        encode_r(Opcode::ADD, 3, 1, 2)
    );

    // PC = 4
    append_instruction(
        program,
        encode_r(Opcode::ADD, 4, 1, 2)
    );

    MemState instruction_memory(program);
    RegFile registers;
    Data_mem data_memory;

    registers.write(1, 10);
    registers.write(2, 10);

    Pipeline pipeline(
        instruction_memory,
        registers,
        data_memory
    );

    pipeline.step();

    assert(instruction_memory.get_pc() == 4);

    std::cout << "BEQ taken test passed.\n";
}

void test_beq_not_taken()
{
    std::cout << "\nTesting BEQ not taken...\n";

    std::vector<std::uint8_t> program;

    append_instruction(
        program,
        encode_beq(1, 2, 4)
    );

    MemState instruction_memory(program);
    RegFile registers;
    Data_mem data_memory;

    registers.write(1, 10);
    registers.write(2, 20);

    Pipeline pipeline(
        instruction_memory,
        registers,
        data_memory
    );

    pipeline.step();

    assert(instruction_memory.get_pc() == 2);

    std::cout << "BEQ not taken test passed.\n";
}

void test_jal()
{
    std::cout << "\nTesting JAL...\n";

    std::vector<std::uint8_t> program;

    // PC = 0: JAL R5, +4
    append_instruction(
        program,
        encode_jal(5, 4)
    );

    // PC = 2
    append_instruction(
        program,
        encode_r(Opcode::ADD, 1, 1, 1)
    );

    // PC = 4
    append_instruction(
        program,
        encode_r(Opcode::ADD, 2, 2, 2)
    );

    MemState instruction_memory(program);
    RegFile registers;
    Data_mem data_memory;

    Pipeline pipeline(
        instruction_memory,
        registers,
        data_memory
    );

    pipeline.step();

    // JAL stores PC + 2 as the return address.
    assert(registers.read(5) == 2);

    // JAL jumps from PC = 0 to PC = 4.
    assert(instruction_memory.get_pc() == 4);

    std::cout << "JAL test passed.\n";
}

int main()
{
    std::cout << "==============================\n";
    std::cout << " Poseidon16 Pipeline Tests\n";
    std::cout << "==============================\n";

    test_alu_instructions();
    test_immediate_instructions();
    test_r0();
    test_beq_taken();
    test_beq_not_taken();
    test_jal();

    std::cout << "\n==============================\n";
    std::cout << "All pipeline tests passed!\n";
    std::cout << "==============================\n";

    return 0;
}