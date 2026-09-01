#include <iostream>
#include <iomanip>
#include <vector>

#include "ISA.h"
#include "program.h"
#include "state.h"

using namespace poseidon16;

int main()
{
    // ------------------------------------------------------------
    // Initialize CPU components
    // ------------------------------------------------------------

    ROM rom(program);

    Decoder decoder;
    Disassembler disassembler;

    RegFile regs;


    // ------------------------------------------------------------
    // Main instruction execution loop
    //
    // FETCH -> DECODE -> DISASSEMBLE -> EXECUTE
    // ------------------------------------------------------------

    while (rom.get_pc() + 1 < program.size())
    {
        // ========================================================
        // 1. FETCH
        // ========================================================

        u32 current_pc = rom.get_pc();

        u16 instruction = rom.fetch16();


        // ========================================================
        // 2. DECODE
        // ========================================================

        DecodedInstruction decoded =
            decoder.decode(instruction);


        // ========================================================
        // 3. DISASSEMBLE
        // ========================================================

        std::string assembly_text =
            disassembler.disassemble(decoded);


        // Print current instruction in real time
        std::cout << "PC: 0x"
                  << std::hex
                  << std::uppercase
                  << std::setw(4)
                  << std::setfill('0')
                  << current_pc

                  << "   Instruction: 0x"
                  << std::setw(4)
                  << instruction

                  << "   "
                  << assembly_text

                 << '\n';


        // ========================================================
        // READ REGISTER OPERANDS
        //
        // Directly using RegFile::read() from state.cpp
        // ========================================================

        ReadyInstruction ready{};

        ready.opcode    = decoded.opcode;
        ready.format    = decoded.format;
        ready.rd        = decoded.rd;
        ready.rs1       = decoded.rs1;
        ready.rs2       = decoded.rs2;
        ready.immediate = decoded.immediate;


        switch (decoded.opcode)
        {
            // ----------------------------------------------------
            // R-Type
            // rd = operation(rs1, rs2)
            // ----------------------------------------------------

            case Opcode::ADD:
            case Opcode::SUB:
            case Opcode::AND:
            case Opcode::OR:
            case Opcode::SLT:
            case Opcode::MUL:
            case Opcode::DIV:
            case Opcode::REM:
            {
                ready.reg1_data =
                    regs.read(decoded.rs1);

                ready.reg2_data =
                    regs.read(decoded.rs2);

                break;
            }


            // ----------------------------------------------------
            // I-Type
            // rd = operation(rs1, immediate)
            // ----------------------------------------------------

            case Opcode::ADDI:
            case Opcode::ANDI:
            case Opcode::ORI:
            case Opcode::SLTI:
            case Opcode::LW:
            {
                ready.reg1_data =
                    regs.read(decoded.rs1);

                ready.reg2_data = 0;

                break;
            }


            // ----------------------------------------------------
            // SW
            //
            // rs1 = address base
            // rs2 = value to store
            // ----------------------------------------------------

            case Opcode::SW:
            {
                ready.reg1_data =
                    regs.read(decoded.rs1);

                ready.reg2_data =
                    regs.read(decoded.rs2);

                break;
            }


            // ----------------------------------------------------
            // BEQ
            //
            // Compare rs1 and rs2
            // ----------------------------------------------------

            case Opcode::BEQ:
            {
                ready.reg1_data =
                    regs.read(decoded.rs1);

                ready.reg2_data =
                    regs.read(decoded.rs2);

                break;
            }


            // ----------------------------------------------------
            // JAL
            //
            // No source-register operand required
            // ----------------------------------------------------

            case Opcode::JAL:
            {
                ready.reg1_data = 0;
                ready.reg2_data = 0;

                break;
            }
        }


        // ========================================================
        // 4. EXECUTE
        // ========================================================

        action result =
            ALU::execute(ready);


        // ========================================================
        // WRITE BACK
        //
        // Directly using RegFile::write() from state.cpp
        // ========================================================

        if (result.flag == 0)
        {
            regs.write(
                result.rd,
                result.result
            );
        }



        // ------------------------------------------------------------
        // Print final register file
        // ------------------------------------------------------------

        std::cout << "========================================\n";
        std::cout << "Final register values:\n";
        std::cout << "========================================\n";

        for (int i = 0; i < 16; ++i)
        {
            std::cout << "R"
                    << std::dec
                    << i
                    << " = "
                    << regs.read(i)
                    << '\n';
        }
        std :: cout << '\n';

        // ========================================================
        // Move to next instruction
        // ========================================================

        rom.next_instr();
    }



    return 0;
}