#include <iostream>
#include <vector>
#include <iomanip>
#include "decoder.h"
#include "disassembler.h"

int main()
{
    Decoder decoder;
    Disassembler disassembler;

    std::vector<uint16_t> test_instructions = {
        0b0000'0001'0010'0011, // ADD  R1, R2, R3
        0b0001'0100'0101'0110, // SUB  R4, R5, R6
        0b1000'0100'0101'1110, // ADDI R4, R5, -2
        0b1101'0010'0001'0100, // SW   R2, R1, 4
        0b1110'0001'0010'1110, // BEQ  R1, R2, -4
        0b1111'0001'0000'0101  // JAL  R1, 10
    };

    std::cout << "==================================================\n";
    std::cout << "          PIPELINE: DECODER -> DISASSEMBLER       \n";
    std::cout << "==================================================\n\n";

    for (size_t i = 0; i < test_instructions.size(); ++i)
    {
        uint16_t raw_bin = test_instructions[i];

        DecodedInstruction decoded = decoder.decode(raw_bin);

        std::string assembly_text = disassembler.disassemble(decoded);

        std::cout << "Test [" << i + 1 << "] Raw Hex: 0x"
                  << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << raw_bin
                  << "  --->  Assembly: " << assembly_text << "\n";
    }

    std::cout << "\n==================================================\n";

    return 0;
}