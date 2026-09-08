# Poseidon16 RISC Core

Poseidon16 is a C++ implementation and simulator of a custom **16-bit RISC processor architecture**.

The project demonstrates the main CPU instruction-processing flow:

```text
Fetch -> Decode -> Disassemble -> Execute -> Write Back
```

The processor is based on the custom **Poseidon16 Instruction Set Architecture (ISA)** and includes arithmetic, logical, comparison, memory, multiplication/division, and control-flow instructions.

---

# 1. Start With the Poseidon16 ISA

Before reading, modifying, compiling, or running the processor implementation, you **must begin by reading the Poseidon16 ISA documentation**.

The ISA documentation is available here:

👉 [Poseidon16 ISA Documentation](https://github.com/mohamedatef2001/poseidon16-risc-core/tree/main/ISA)

The ISA documentation defines the architecture of Poseidon16, including:

- Register file
- Instruction length
- Instruction encoding
- Opcodes
- Immediate values
- Memory organization
- R-Type instructions
- I-Type instructions
- C-Type instructions
- Branch and jump behavior
- M-extension instructions

---

## Poseidon16 Architecture Overview

Poseidon16 contains:

- **16 general-purpose registers:** `R0` to `R15`
- **32-bit register width**
- **16-bit fixed-length instructions**
- **2 bytes per instruction**
- **4-bit primary opcode**
- **Big-Endian instruction and memory organization**

Register `R0` is hardwired to zero.

```text
R0 = 0
```

Reading from `R0` always returns `0`, and writing to `R0` has no effect.

Every Poseidon16 instruction has a fixed length of:

```text
16 bits = 2 bytes
```

The first four bits represent the primary opcode:

```text
Instruction[15:12] = Opcode
```

The remaining twelve bits are interpreted according to the instruction format.

Poseidon16 uses three main instruction formats:

```text
R-Type
I-Type
C-Type
```

---

# R-Type Instruction Format

R-Type instructions are used when an instruction operates on two source registers and writes the result into a destination register.

The 16-bit R-Type instruction is divided into four 4-bit fields:

```text
15          12 11           8 7            4 3            0
+--------------+--------------+--------------+--------------+
|    Opcode    |      rd      |     rs1      |     rs2      |
+--------------+--------------+--------------+--------------+
     4 bits         4 bits         4 bits         4 bits
```

The fields are:

```text
Opcode = Instruction[15:12]
rd     = Instruction[11:8]
rs1    = Instruction[7:4]
rs2    = Instruction[3:0]
```

Where:

- `rd` is the destination register.
- `rs1` is the first source register.
- `rs2` is the second source register.

### R-Type Instructions

Poseidon16 uses R-Type encoding for:

```text
ADD
SUB
AND
OR
SLT
MUL
DIV
REM
```

### Example

```asm
ADD R2, R3, R4
```

This instruction performs:

```text
R2 = R3 + R4
```

The instruction encoding is:

```text
0000 0010 0011 0100
```

Hexadecimal:

```text
0x0234
```

The four fields are:

```text
0000 -> ADD
0010 -> R2
0011 -> R3
0100 -> R4
```

Therefore:

```text
0x0234 -> ADD R2, R3, R4
```

---

# I-Type Instruction Format

I-Type instructions are used for:

- Register-immediate operations
- Load instructions
- Store instructions

The general format is:

```text
15          12 11           8 7            4 3            0
+--------------+--------------+--------------+--------------+
|    Opcode    |   rd / rs2   |     rs1      |     imm4     |
+--------------+--------------+--------------+--------------+
     4 bits         4 bits         4 bits         4 bits
```

For most I-Type instructions:

```text
Opcode = Instruction[15:12]
rd     = Instruction[11:8]
rs1    = Instruction[7:4]
imm4   = Instruction[3:0]
```

The immediate value is a **signed 4-bit value**.

Its range is:

```text
-8 to +7
```

The immediate value is sign-extended to 32 bits before being used.

### I-Type Instructions

```text
ADDI
ANDI
ORI
SLTI
LW
SW
```

---

## Example: ADDI

```asm
ADDI R2, R3, -3
```

The operation is:

```text
R2 = R3 + (-3)
```

Encoding:

```text
1000 0010 0011 1101
```

Hexadecimal:

```text
0x823D
```

The fields are:

```text
1000 -> ADDI
0010 -> R2
0011 -> R3
1101 -> -3
```

Therefore:

```text
0x823D -> ADDI R2, R3, -3
```

---

## Special Case: SW

`SW` uses the same I-Type layout, but bits `[11:8]` represent `rs2` instead of `rd`.

```text
15          12 11           8 7            4 3            0
+--------------+--------------+--------------+--------------+
|    Opcode    |     rs2      |     rs1      |     imm4     |
+--------------+--------------+--------------+--------------+
```

For example:

```asm
SW R2, 4(R3)
```

The fields represent:

```text
rs2  = R2
rs1  = R3
imm4 = 4
```

The effective memory address is calculated using:

```text
EA = R[rs1] + SignExt(imm4)
```

In this example:

```text
EA = R3 + 4
```

and the value stored in `R2` is written to memory.

---

# C-Type Instruction Format

C-Type instructions are used for **control-flow operations**.

Poseidon16 defines two C-Type instructions:

```text
BEQ
JAL
```

The general C-Type format is:

```text
15          12 11           8 7            4 3            0
+--------------+--------------+--------------+--------------+
|    Opcode    |      A       |      B       |      C       |
+--------------+--------------+--------------+--------------+
```

The meaning of `A`, `B`, and `C` depends on the instruction.

---

## BEQ

For `BEQ`:

```text
A = rs1
B = rs2
C = off4
```

The format becomes:

```text
15          12 11           8 7            4 3            0
+--------------+--------------+--------------+--------------+
|     1110     |     rs1      |     rs2      |     off4     |
+--------------+--------------+--------------+--------------+
```

Example:

```asm
BEQ R1, R2, +6
```

The processor compares:

```text
R1 == R2
```

If the registers are equal, the branch is taken.

Poseidon16 stores branch offsets in units of 2-byte instructions.

Therefore:

```text
byte_offset = SignExt(off4) << 1
```

If the branch is not taken, the processor continues to the next instruction:

```text
PC = PC + 2
```

---

## JAL

`JAL` uses an 8-bit signed offset.

Its format is:

```text
15          12 11           8 7                           0
+--------------+--------------+-----------------------------+
|     1111     |      rd      |            off8             |
+--------------+--------------+-----------------------------+
```

Where:

```text
rd   = link register
off8 = signed jump offset
```

The operation is:

```text
R[rd] = PC + 2
PC = PC + (SignExt(off8) << 1)
```

The address of the next instruction is stored in `rd` before the jump occurs.

Example:

```asm
JAL R1, +20
```

This saves the return address in `R1` and then updates the program counter to the PC-relative jump target.

---

# Poseidon16 Instruction Set

The Poseidon16 ISA currently defines the following instructions:

| Opcode | Instruction | Type | Description |
|--------|-------------|------|-------------|
| `0x0` | ADD | R-Type | Register addition |
| `0x1` | SUB | R-Type | Register subtraction |
| `0x2` | AND | R-Type | Bitwise AND |
| `0x3` | OR | R-Type | Bitwise OR |
| `0x4` | SLT | R-Type | Signed set-less-than |
| `0x5` | MUL | R-Type | Signed multiplication |
| `0x6` | DIV | R-Type | Signed division |
| `0x7` | REM | R-Type | Signed remainder |
| `0x8` | ADDI | I-Type | Add immediate |
| `0x9` | ANDI | I-Type | AND immediate |
| `0xA` | ORI | I-Type | OR immediate |
| `0xB` | SLTI | I-Type | Set-less-than immediate |
| `0xC` | LW | I-Type | Load word |
| `0xD` | SW | I-Type | Store word |
| `0xE` | BEQ | C-Type | Branch if equal |
| `0xF` | JAL | C-Type | Jump and link |

---

# 2. Final Integrated Files

The final integrated version of the Poseidon16 processor is located in the `integration_R` directory.

👉 [Poseidon16 Final Integration Files](https://github.com/mohamedatef2001/poseidon16-risc-core/tree/main/integration_R)

This directory contains the files required to compile and execute the integrated processor.

The main files are:

```text
final_main.cpp
ISA.h
program.h
rom.cpp
decoder.cpp
disassembler.cpp
alu.cpp
state.cpp
makefile.txt
```

---

# Project Components

## `final_main.cpp`

`final_main.cpp` connects the main components of the processor and controls the execution flow.

The main processing sequence is:

```text
FETCH
  |
  v
DECODE
  |
  v
DISASSEMBLE
  |
  v
READ OPERANDS
  |
  v
EXECUTE
  |
  v
WRITE BACK
```

---

## `ISA.h`

`ISA.h` contains the central definitions used throughout the processor implementation.

It defines:

- Data types
- Register count
- Register width
- Instruction width
- Opcodes
- Instruction formats
- Instruction masks
- Instruction field positions
- Decoded instruction structures
- Ready instruction structures
- ALU action structures
- Decoder interface
- Disassembler interface
- ALU interface
- ROM interface

---

## `rom.cpp`

`rom.cpp` implements the instruction ROM and Fetch stage.

The program is stored as individual bytes.

Because Poseidon16 uses Big-Endian instruction storage, fetching a 16-bit instruction combines two consecutive bytes as:

```text
instruction[15:8] = memory[PC]
instruction[7:0]  = memory[PC + 1]
```

For example:

```text
memory[0] = 0x81
memory[1] = 0x05
```

produces:

```text
0x8105
```

Since each instruction occupies two bytes, normal sequential execution advances the program counter by:

```text
PC = PC + 2
```

---

## `decoder.cpp`

The decoder receives the raw 16-bit instruction produced by the Fetch stage.

It extracts the required instruction fields according to the Poseidon16 ISA.

These fields include:

```text
Opcode
Format
rd
rs1
rs2
Immediate
Offset
```

For example:

```text
0x0312
```

is separated into:

```text
Opcode = 0x0
rd     = R3
rs1    = R1
rs2    = R2
```

which represents:

```asm
ADD R3, R1, R2
```

The decoder also handles sign extension for immediate and control-flow values.

---

## `disassembler.cpp`

The disassembler converts a decoded instruction into a human-readable assembly representation.

For example:

```text
Raw Instruction:

0x0312
```

is converted into:

```asm
ADD R3, R1, R2
```

This allows the user to observe the currently executing processor instruction in real time.

---

## `alu.cpp`

The ALU performs the arithmetic and logical operations required by the instruction.

The ALU currently handles operations including:

```text
ADD
SUB
AND
OR
SLT
MUL
DIV
REM
ADDI
ANDI
ORI
```

The ALU is also responsible for calculating effective addresses for:

```text
LW
SW
```

The ALU receives a `ReadyInstruction` containing both the decoded instruction information and the values read from the register file.

---

## `state.cpp`

`state.cpp` contains the processor register-file implementation.

Poseidon16 contains:

```text
R0 - R15
```

Each register is:

```text
32 bits
```

Register `R0` is hardwired to zero:

```text
R0 = 0
```

Reading `R0` always returns zero.

Writing to `R0` is ignored.

---

## `program.h`

`program.h` contains the program that is loaded into the Poseidon16 ROM.

Instructions are represented as individual bytes in **Big-Endian order**.

For example:

```cpp
0x81, 0x05
```

represents the 16-bit instruction:

```text
0x8105
```

which is decoded as:

```asm
ADDI R1, R0, 5
```

---

# 3. Clone the Repository

Before compiling the project, make sure that you have:

- Git
- C++ compiler
- `g++`
- C++17 support
- Visual Studio Code, if you want to use the same development environment

Clone the repository using:

```bash
git clone https://github.com/mohamedatef2001/poseidon16-risc-core.git
```

Then move into the repository:

```bash
cd poseidon16-risc-core
```

Move into the final integration directory:

```bash
cd integration_R
```

You should now be inside:

```text
poseidon16-risc-core/integration_R
```

The same folder can be opened directly from GitHub here:

👉 [Poseidon16 integration_R](https://github.com/mohamedatef2001/poseidon16-risc-core/tree/main/integration_R)

---

# 4. Compile and Run

The required compilation and execution commands are also available in:

```text
makefile.txt
```

inside the `integration_R` directory.

If you are using **Visual Studio Code**, open the project and use:

```text
Ctrl + Alt + N
```

to run using the Code Runner extension.

You can also compile and run the project directly from the terminal.

---

## Step 1 - Compile

Inside the `integration_R` directory, run:

```bash
g++ -std=c++17 final_main.cpp rom.cpp decoder.cpp alu.cpp disassembler.cpp state.cpp -o execute_test.exe
```

This command compiles:

```text
final_main.cpp
rom.cpp
decoder.cpp
alu.cpp
disassembler.cpp
state.cpp
```

and creates the executable:

```text
execute_test.exe
```

---

## Step 2 - Run

After the compilation completes successfully, run:

```bash
./execute_test.exe
```

If you are using Windows PowerShell, you can also run:

```powershell
.\execute_test.exe
```

---

# Processor Execution Flow

When the program runs, each instruction passes through the main processor stages.

```text
              +----------------+
              |      ROM       |
              +-------+--------+
                      |
                      | Fetch
                      v
              +----------------+
              |    Decoder     |
              +-------+--------+
                      |
                      | DecodedInstruction
                      v
              +----------------+
              | Disassembler   |
              +-------+--------+
                      |
                      | Human-readable instruction
                      v
              +----------------+
              | Read Operands  |
              +-------+--------+
                      |
                      | ReadyInstruction
                      v
              +----------------+
              |      ALU       |
              |    Execute     |
              +-------+--------+
                      |
                      | Execution result
                      v
              +----------------+
              |   Register     |
              |     File       |
              +----------------+
```

The main CPU processing sequence is therefore:

```text
Fetch -> Decode -> Disassemble -> Read Operands -> Execute -> Write Back
```

---

# Example Execution

The processor prints the current instruction while the program executes.

Example:

```text
PC: 0x0000   Instruction: 0x8105   ADDI R1, R0, 5
PC: 0x0002   Instruction: 0x8203   ADDI R2, R0, 3
PC: 0x0004   Instruction: 0x0312   ADD R3, R1, R2
```

This allows the user to observe:

- Current Program Counter
- Raw instruction
- Disassembled instruction
- Register updates

during execution.

---

# Example Poseidon16 Program

An example program can contain:

```asm
ADDI R1, R0, 5
ADDI R2, R0, 3
ADD  R3, R1, R2
SUB  R4, R3, R2
ANDI R5, R4, 7
OR   R6, R5, R1
MUL  R7, R1, R2
```

The expected register values are:

```text
R1 = 5
R2 = 3
R3 = 8
R4 = 5
R5 = 5
R6 = 5
R7 = 15
```

---

