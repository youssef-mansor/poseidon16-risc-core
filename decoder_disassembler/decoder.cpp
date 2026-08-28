#include "decoder.h"

static int32_t sign_extend_4(uint8_t value)
{
    if (value & 0x8)
        return value | 0xFFFFFFF0;

    return value;
}

static int32_t sign_extend_8(uint8_t value)
{
    if (value & 0x80)
        return value | 0xFFFFFF00;

    return value;
}

DecodedInstruction Decoder::decode(uint16_t instruction)
{
    DecodedInstruction decoded{};

    // Extract opcode [15:12]
    uint8_t op = (instruction >> 12) & 0xF;

    // Save opcode
    decoded.opcode = static_cast<Opcode>(op);

    // R-Type
    if (op <= 0x7) {
        decoded.format = Format::R;

        decoded.rd  = (instruction >> 8) & 0xF;
        decoded.rs1 = (instruction >> 4) & 0xF;
        decoded.rs2 = instruction & 0xF;
    }

    // I-Type
    else if (op <= 0xD) {
        decoded.format = Format::I;

        decoded.rs1 = (instruction >> 4) & 0xF;

        if (op == 0xD) {
            // SW
            decoded.rs2 = (instruction >> 8) & 0xF;
        }
        else {
            // ADDI, ANDI, ORI, SLTI, LW
            decoded.rd = (instruction >> 8) & 0xF;
        }

        uint8_t imm4 = instruction & 0xF;
        decoded.immediate = sign_extend_4(imm4);
    }

    // C-Type
    else {
        decoded.format = Format::C;

        if (op == 0xE) {
            // BEQ
            decoded.rs1 = (instruction >> 8) & 0xF;
            decoded.rs2 = (instruction >> 4) & 0xF;

            uint8_t off4 = instruction & 0xF;
            decoded.immediate = sign_extend_4(off4) << 1;
        }
        else {
            // JAL
            decoded.rd = (instruction >> 8) & 0xF;

            uint8_t off8 = instruction & 0xFF;
            decoded.immediate = sign_extend_8(off8) << 1;
        }
    }

    return decoded;
}