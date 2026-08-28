#ifndef DECODER_H
#define DECODER_H

#include <cstdint>

enum class Opcode : uint8_t {
    ADD = 0x0,
    SUB = 0x1,
    AND = 0x2,
    OR = 0x3,
    SLT = 0x4,
    MUL = 0x5,
    DIV = 0x6,
    REM = 0x7,

    ADDI = 0x8,
    ANDI = 0x9,
    ORI = 0xA,
    SLTI = 0xB,
    LW = 0xC,
    SW = 0xD,

    BEQ = 0xE,
    JAL = 0xF
};

enum class Format {
    R,
    I,
    C
};

struct DecodedInstruction {
    Opcode opcode;
    Format format;

    uint8_t rd;
    uint8_t rs1;
    uint8_t rs2;

    int32_t immediate;
};

class Decoder {
public:
    DecodedInstruction decode(uint16_t instruction);
};

#endif