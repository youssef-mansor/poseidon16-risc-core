#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <string>
#include "decoder.h"

class Disassembler
{
public:
    std::string disassemble(const DecodedInstruction &instruction);
};

#endif