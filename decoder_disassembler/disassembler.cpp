#include "disassembler.h"

static std::string opcode_to_string(Opcode opcode)
{
    switch (opcode)
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
    }

    return "UNKNOWN";
}

std::string Disassembler::disassemble(
    const DecodedInstruction &instruction)
{
    switch (instruction.format)
    {
    // =========================
    // R-Type
    // =========================
    case Format::R:
        return opcode_to_string(instruction.opcode) + " R" + std::to_string(instruction.rd) + ", R" + std::to_string(instruction.rs1) + ", R" + std::to_string(instruction.rs2);

    // =========================
    // I-Type
    // =========================
    case Format::I:

        if (instruction.opcode == Opcode::SW)
        {
            return "SW R" + std::to_string(instruction.rs2) + ", R" + std::to_string(instruction.rs1) + ", " + std::to_string(instruction.immediate);
        }

        return opcode_to_string(instruction.opcode) + " R" + std::to_string(instruction.rd) + ", R" + std::to_string(instruction.rs1) + ", " + std::to_string(instruction.immediate);

    // =========================
    // C-Type
    // =========================
    case Format::C:

        if (instruction.opcode == Opcode::BEQ)
        {
            return "BEQ R" + std::to_string(instruction.rs1) + ", R" + std::to_string(instruction.rs2) + ", " + std::to_string(instruction.immediate);
        }

        return "JAL R" + std::to_string(instruction.rd) + ", " + std::to_string(instruction.immediate);
    }

    return "UNKNOWN";
}