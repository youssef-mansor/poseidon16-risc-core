#include "ISA.h"

namespace poseidon16
{

    static std::int32_t sign_extend_4(std::uint8_t value)
    {
        if (value & 0x8)
            return value | 0xFFFFFFF0;
        return value;
    }

    static std::int32_t sign_extend_8(std::uint8_t value)
    {
        if (value & 0x80)
            return value | 0xFFFFFF00;
        return value;
    }

    DecodedInstruction Decoder::decode(std::uint16_t instruction)
    {
        DecodedInstruction decoded{};

        // Extract opcode using ISA constants
        std::uint8_t op = (instruction & OPCODE_MASK) >> OPCODE_SHIFT;
        decoded.opcode = static_cast<Opcode>(op);

        // R-Type
        if (op <= 0x7)
        {
            decoded.format = Format::R;
            decoded.rd = (instruction & FIELD_A_MASK) >> FIELD_A_SHIFT;
            decoded.rs1 = (instruction & FIELD_B_MASK) >> FIELD_B_SHIFT;
            decoded.rs2 = instruction & FIELD_C_MASK;
        }
        // I-Type
        else if (op <= 0xD)
        {
            decoded.format = Format::I;
            decoded.rs1 = (instruction & FIELD_B_MASK) >> FIELD_B_SHIFT;

            if (decoded.opcode == Opcode::SW)
            {
                decoded.rs2 = (instruction & FIELD_A_MASK) >> FIELD_A_SHIFT;
            }
            else
            {
                decoded.rd = (instruction & FIELD_A_MASK) >> FIELD_A_SHIFT;
            }

            std::uint8_t imm4 = instruction & FIELD_C_MASK;
            decoded.immediate = sign_extend_4(imm4);
        }
        // C-Type
        else
        {
            decoded.format = Format::C;

            if (decoded.opcode == Opcode::BEQ)
            {
                decoded.rs1 = (instruction & FIELD_A_MASK) >> FIELD_A_SHIFT;
                decoded.rs2 = (instruction & FIELD_B_MASK) >> FIELD_B_SHIFT;

                std::uint8_t off4 = instruction & FIELD_C_MASK;
                decoded.immediate = sign_extend_4(off4) * CONTROL_OFFSET_SCALE;
            }
            else
            { // JAL
                decoded.rd = (instruction & FIELD_A_MASK) >> FIELD_A_SHIFT;

                std::uint8_t off8 = instruction & OFF8_MASK;
                decoded.immediate = sign_extend_8(off8) * CONTROL_OFFSET_SCALE;
            }
        }

        return decoded;
    }

}