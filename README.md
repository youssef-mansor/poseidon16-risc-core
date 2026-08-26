# Poseidon16 Processor Emulator

A C++ architectural simulator and disassembler for the Poseidon16 custom 16-bit RISC instruction set architecture.

## Architectural Specifications

- **Instruction Width:** Fixed 16-bit encoding
- **Register File:** 16 general-purpose 32-bit registers (`R0`–`R15`, where `R0` is hardwired to 0)
- **Memory Model:** Byte-addressable, Big-Endian byte order
- **Memory Alignment:** Misaligned 32-bit loads and stores are explicitly supported
- **Instruction Support:** 
  - Base (I) Extension: `ADD`, `ADDI`, `SUB`, `AND`, `ANDI`, `OR`, `ORI`, `SLT`, `SLTI`, `LW`, `SW`, `BEQ`, `JAL`
  - Integer (M) Extension: `MUL`, `DIV`, `REM` (with divide-by-zero edge handling)

## Pipeline Architecture

The emulator runs a synchronous 4-stage processing loop on every cycle:
1. **Fetch:** Retrieves 16-bit instruction from Big-Endian memory.
2. **Decode:** Extracts opcodes, register indices, and sign-extended immediates across 3 instruction formats.
3. **Disassemble:** Streams human-readable assembly instructions to stdout in real time.
4. **Execute:** Resolves ALU/M-extension math, unaligned memory operations, and state/PC updates.

## Quickstart

### Build
