#ifndef POSEIDON16_PROGRAM_H
#define POSEIDON16_PROGRAM_H

#include <cstdint>
#include <vector>

// Example Poseidon16 program.
//
// Instruction sequence:
//   0x8105 = ADDI R1, R0, 5
//   0x8203 = ADDI R2, R0, 3
//   0x0312 = ADD  R3, R1, R2
//   0x1432 = SUB  R4, R3, R2
//   0x9547 = ANDI R5, R4, 7
//   0x3651 = OR   R6, R5, R1
//   0x5712 = MUL  R7, R1, R2
//
// Expected register results once execution is connected:
//   R1 = 5
//   R2 = 3
//   R3 = 8
//   R4 = 5
//   R5 = 5
//   R6 = 5
//   R7 = 15
//
// Instructions are stored as bytes in Big-Endian order.

inline const std::vector<std::uint8_t> program = {
    0x81, 0x05, // ADDI R1, R0, 5
    0x82, 0x03, // ADDI R2, R0, 3
    0x03, 0x12, // ADD  R3, R1, R2
    0x14, 0x32, // SUB  R4, R3, R2
    0x95, 0x47, // ANDI R5, R4, 7
    0x36, 0x51, // OR   R6, R5, R1
    0x57, 0x12  // MUL  R7, R1, R2
};

#endif