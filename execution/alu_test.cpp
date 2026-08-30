#include "alu.h"

#include <cassert>
#include <cstdint>
#include <iostream>

using namespace poseidon16;

void test_add()
{
    std::cout << "Testing ADD...\n";

    assert(ALU::execute(Opcode::ADD, 5, 3) == 8);
    assert(ALU::execute(Opcode::ADD, 10, 20) == 30);
    assert(ALU::execute(Opcode::ADD, 0, 0) == 0);

    std::cout << "  ADD tests passed.\n";
}

void test_sub()
{
    std::cout << "Testing SUB...\n";

    assert(ALU::execute(Opcode::SUB, 5, 3) == 2);
    assert(ALU::execute(Opcode::SUB, 3, 5) == -2);
    assert(ALU::execute(Opcode::SUB, 10, 10) == 0);

    std::cout << "  SUB tests passed.\n";
}

void test_negative_arithmetic()
{
    std::cout << "Testing negative arithmetic...\n";

    // -5 + 3 = -2
    assert(ALU::execute(Opcode::ADD, -5, 3) == -2);

    // 5 + (-3) = 2
    assert(ALU::execute(Opcode::ADD, 5, -3) == 2);

    // -5 - 3 = -8
    assert(ALU::execute(Opcode::SUB, -5, 3) == -8);

    // 5 - (-3) = 8
    assert(ALU::execute(Opcode::SUB, 5, -3) == 8);

    // -5 - (-3) = -2
    assert(ALU::execute(Opcode::SUB, -5, -3) == -2);

    std::cout << "  Negative arithmetic tests passed.\n";
}

void test_and()
{
    std::cout << "Testing AND...\n";

    // 1100 & 1010 = 1000
    assert(ALU::execute(Opcode::AND, 0b1100, 0b1010) == 0b1000);

    // 1111 & 1010 = 1010
    assert(ALU::execute(Opcode::AND, 0b1111, 0b1010) == 0b1010);

    // Anything AND 0 = 0
    assert(ALU::execute(Opcode::AND, 0xFFFFFFFF, 0) == 0);

    std::cout << "  AND tests passed.\n";
}

void test_or()
{
    std::cout << "Testing OR...\n";

    // 1100 | 1010 = 1110
    assert(ALU::execute(Opcode::OR, 0b1100, 0b1010) == 0b1110);

    // 0000 | 1010 = 1010
    assert(ALU::execute(Opcode::OR, 0, 0b1010) == 0b1010);

    std::cout << "  OR tests passed.\n";
}

void test_slt()
{
    std::cout << "Testing SLT...\n";

    // Positive numbers
    assert(ALU::execute(Opcode::SLT, 5, 10) == 1);
    assert(ALU::execute(Opcode::SLT, 10, 5) == 0);

    // Equal values
    assert(ALU::execute(Opcode::SLT, 5, 5) == 0);

    // Signed comparison
    assert(ALU::execute(Opcode::SLT, -5, 3) == 1);
    assert(ALU::execute(Opcode::SLT, 3, -5) == 0);

    // Two negative values
    assert(ALU::execute(Opcode::SLT, -10, -5) == 1);
    assert(ALU::execute(Opcode::SLT, -5, -10) == 0);

    // Zero
    assert(ALU::execute(Opcode::SLT, -1, 0) == 1);
    assert(ALU::execute(Opcode::SLT, 0, -1) == 0);

    std::cout << "  SLT tests passed.\n";
}

void test_immediate_style_operations()
{
    std::cout << "Testing immediate-style ALU operations...\n";

    /*
     * The ALU does not have separate ADDI/ANDI/ORI/SLTI
     * operations. The pipeline will use the corresponding
     * base ALU opcode and provide the immediate as operand b.
     *
     * ADDI R3, R1, 5
     * where R1 = 10
     */
    assert(ALU::execute(Opcode::ADD, 10, 5) == 15);

    /*
     * ANDI R3, R1, 0x0F
     */
    assert(ALU::execute(Opcode::AND, 0xF0, 0x0F) == 0x00);

    /*
     * ORI R3, R1, 0x0F
     */
    assert(ALU::execute(Opcode::OR, 0xF0, 0x0F) == 0xFF);

    /*
     * SLTI R3, R1, 5
     * where R1 = 3
     */
    assert(ALU::execute(Opcode::SLT, 3, 5) == 1);

    /*
     * SLTI R3, R1, 5
     * where R1 = 10
     */
    assert(ALU::execute(Opcode::SLT, 10, 5) == 0);

    std::cout << "  Immediate-style tests passed.\n";
}

void test_zero_operands()
{
    std::cout << "Testing zero operands...\n";

    assert(ALU::execute(Opcode::ADD, 0, 5) == 5);
    assert(ALU::execute(Opcode::ADD, 5, 0) == 5);

    assert(ALU::execute(Opcode::SUB, 0, 5) == -5);
    assert(ALU::execute(Opcode::SUB, 5, 0) == 5);

    assert(ALU::execute(Opcode::AND, 0, 0xFFFFFFFF) == 0);
    assert(ALU::execute(Opcode::OR, 0, 5) == 5);

    std::cout << "  Zero operand tests passed.\n";
}

int main()
{
    std::cout << "====================================\n";
    std::cout << "       Poseidon16 ALU Tests\n";
    std::cout << "====================================\n\n";

    test_add();
    test_sub();
    test_negative_arithmetic();
    test_and();
    test_or();
    test_slt();
    test_immediate_style_operations();
    test_zero_operands();

    std::cout << "\n====================================\n";
    std::cout << "       ALL ALU TESTS PASSED\n";
    std::cout << "====================================\n";

    return 0;
}