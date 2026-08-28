#include <iostream>
#include <iomanip>
#include <string>
#include "decoder.h"

std::string getOpcodeName(Opcode op)
{
    switch (op)
    {
    case Opcode::ADD:
        return "ADD";
    case Opcode::SUB:
        return "SUB";
    case Opcode::AND:
        return "AND";
    case Opcode::OR:
        return "OR";
    case Opcode::SLT:
        return "SLT";
    case Opcode::MUL:
        return "MUL";
    case Opcode::DIV:
        return "DIV";
    case Opcode::REM:
        return "REM";
    case Opcode::ADDI:
        return "ADDI";
    case Opcode::ANDI:
        return "ANDI";
    case Opcode::ORI:
        return "ORI";
    case Opcode::SLTI:
        return "SLTI";
    case Opcode::LW:
        return "LW";
    case Opcode::SW:
        return "SW";
    case Opcode::BEQ:
        return "BEQ";
    case Opcode::JAL:
        return "JAL";
    default:
        return "UNKNOWN";
    }
}

void printDecoded(uint16_t raw_inst, const DecodedInstruction &d)
{
    std::cout << "-------------------------------------------\n";
    std::cout << "Instruction Hex: 0x" << std::hex << std::uppercase << raw_inst << std::dec << "\n";
    std::cout << "Opcode: " << getOpcodeName(d.opcode) << "\n";

    if (d.format == Format::R)
    {
        std::cout << "Format: R-Type\n";
        std::cout << "rd  = r" << (int)d.rd << "\n";
        std::cout << "rs1 = r" << (int)d.rs1 << "\n";
        std::cout << "rs2 = r" << (int)d.rs2 << "\n";
    }
    else if (d.format == Format::I)
    {
        std::cout << "Format: I-Type\n";
        if (d.opcode == Opcode::SW)
        {
            std::cout << "rs2 = r" << (int)d.rs2 << "\n";
        }
        else
        {
            std::cout << "rd  = r" << (int)d.rd << "\n";
        }
        std::cout << "rs1 = r" << (int)d.rs1 << "\n";
        std::cout << "Immediate = " << d.immediate << "\n";
    }
    else
    {
        std::cout << "Format: C-Type\n";
        if (d.opcode == Opcode::BEQ)
        {
            std::cout << "rs1 = r" << (int)d.rs1 << "\n";
            std::cout << "rs2 = r" << (int)d.rs2 << "\n";
        }
        else
        {
            std::cout << "rd  = r" << (int)d.rd << "\n";
        }
        std::cout << "Offset = " << d.immediate << "\n";
    }
}

int main()
{
    Decoder decoder;

    // R-Type -> ADD r1, r2, r3 (Opcode=0x0, rd=1, rs1=2, rs2=3)
    // Binary: 0000 0001 0010 0011 -> Hex: 0x0123
    uint16_t test1 = 0x0123;

    // I-Type -> ADDI r4, r5, -2 (Opcode=0x8, rd=4, rs1=5, imm4=0xE which is -2)
    // Binary: 1000 0100 0101 1110 -> Hex: 0x845E
    uint16_t test2 = 0x845E;

    // C-Type -> BEQ r1, r2, offset (Opcode=0xE, rs1=1, rs2=2, off4=0xF)
    // Binary: 1110 0001 0010 1111 -> Hex: 0xE12F
    uint16_t test3 = 0xE12F;

    // C-Type -> JAL r3, offset (Opcode=0xF, rd=3, off8=0xFE)
    // Binary: 1111 0011 1111 1110 -> Hex: 0xF3FE
    uint16_t test4 = 0xF3FE;

    printDecoded(test1, decoder.decode(test1));
    printDecoded(test2, decoder.decode(test2));
    printDecoded(test3, decoder.decode(test3));
    printDecoded(test4, decoder.decode(test4));

    return 0;
}