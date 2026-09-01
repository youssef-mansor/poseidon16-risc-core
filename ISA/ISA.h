#ifndef POSEIDON16_ISA_H
#define POSEIDON16_ISA_H

#include <cstdint>
#include <string>

namespace poseidon16
{

    // ------------------------------------------------------------
    // Basic architecture parameters
    // ------------------------------------------------------------
    constexpr std::uint32_t NUM_REGISTERS = 16;
    constexpr std::uint32_t REGISTER_BITS = 32;
    constexpr std::uint32_t INSTRUCTION_BITS = 16;
    constexpr std::uint32_t INSTRUCTION_BYTES = 2;
    constexpr std::uint32_t WORD_BYTES = 4;

    constexpr std::uint32_t ZERO_REGISTER = 0; // R0
    constexpr bool BIG_ENDIAN_MEMORY = true;
    constexpr bool MISALIGNED_MEMORY_ALLOWED = true;

    // ------------------------------------------------------------
    // Instruction opcodes: instruction[15:12]
    // ------------------------------------------------------------
    enum class Opcode : std::uint8_t
    {
        ADD = 0x0,
        SUB = 0x1,
        AND = 0x2,
        OR  = 0x3,
        SLT  = 0x4,
        MUL  = 0x5,
        DIV  = 0x6,
        REM  = 0x7,
        ADDI = 0x8,
        ANDI = 0x9,
        ORI = 0xA,
        SLTI = 0xB,
        LW = 0xC,
        SW = 0xD,

        BEQ = 0xE,
        JAL = 0xF
    };

    // Formats
    enum class Format
    {
        R,
        I,
        C
    };

    // Struct representation of decoded instruction
    struct DecodedInstruction
    {
        Opcode opcode;
        Format format;

        std::uint8_t rd;
        std::uint8_t rs1;
        std::uint8_t rs2;

        std::int32_t immediate;
    };

    // Classes
    class Decoder
    {
    public:
        DecodedInstruction decode(std::uint16_t instruction);
    };

    class Disassembler
    {
    public:
        std::string disassemble(const DecodedInstruction &instruction);
    };

    // ------------------------------------------------------------
    // 16-bit instruction field layout
    // OPCODE_MASK | FIELD_A_MASK | FIELD_B_MASK | FIELD_C_MASK
    // 
    // R-Type:
    // [15:12] opcode | [11:8] rd | [7:4] rs1 | [3:0] rs2
    //
    // I-Type:
    // [15:12] opcode | [11:8] rd/rs2 | [7:4] rs1 | [3:0] imm4
    //
    // C-Type BEQ:
    // [15:12] opcode | [11:8] rs1 | [7:4] rs2 | [3:0] off4
    //
    // C-Type JAL:
    // [15:12] opcode | [11:8] rd | [7:0] off8
    // ------------------------------------------------------------
    constexpr std::uint16_t OPCODE_MASK  = 0xF000;
    constexpr std::uint16_t FIELD_A_MASK = 0x0F00;
    constexpr std::uint16_t FIELD_B_MASK = 0x00F0;
    constexpr std::uint16_t FIELD_C_MASK = 0x000F;
    constexpr std::uint16_t OFF8_MASK    = 0x00FF;

    constexpr std::uint8_t OPCODE_SHIFT  = 12;
    constexpr std::uint8_t FIELD_A_SHIFT = 8;
    constexpr std::uint8_t FIELD_B_SHIFT = 4;

    // ------------------------------------------------------------
    // Immediate / offset properties
    // ------------------------------------------------------------
    constexpr std::int32_t IMM4_MIN = -8;
    constexpr std::int32_t IMM4_MAX = 7;

    constexpr std::int32_t BEQ_OFFSET_MIN_BYTES = -16;
    constexpr std::int32_t BEQ_OFFSET_MAX_BYTES = 14;

    constexpr std::int32_t JAL_OFFSET_MIN_BYTES = -256;
    constexpr std::int32_t JAL_OFFSET_MAX_BYTES = 254;

    // Branch/JAL encoded offsets are stored in 2-byte instruction units.
    constexpr std::int32_t CONTROL_OFFSET_SCALE = 2;

    // ------------------------------------------------------------
    // Defined special results
    // ------------------------------------------------------------
    constexpr std::uint32_t DIV_BY_ZERO_RESULT = 0xFFFFFFFFu;

}

#endif