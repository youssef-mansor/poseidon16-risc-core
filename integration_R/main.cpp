#include <iostream>
#include <iomanip>
#include <vector>
#include "ISA.h"
#include "program.h"
#include "state.h"

using namespace poseidon16;

int main()
{
    // load our program to rom 
    ROM rom(program);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////// Test 1 for fetch ///////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // // Fetch the first instruction at PC = 0.
    // u16 instruction = rom.fetch16();

    // std::cout << "PC: 0x"
    //           << std::hex << std::uppercase << std::setw(4)
    //           << std::setfill('0') << rom.get_pc()
    //           << "  Fetched instruction: 0x"
    //           << std::setw(4) << instruction << '\n';

    // // Move to the next 16-bit instruction and fetch again.
    // rom.next_instr();
    // instruction = rom.fetch16();

    // std::cout << "PC: 0x"
    //           << std::setw(4) << rom.get_pc()
    //           << "  Fetched instruction: 0x"
    //           << std::setw(4) << instruction << '\n';

    Decoder decoder; 
    /*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////Test 2 Fetch -> Decode  ///////////////////////////////////////////////////
    *////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // while (rom.get_pc() < program.size())
    // {
    //     // 1. FETCH
    //     u16 instruction = rom.fetch16();
    //     std::cout << "--------------------------------------------------\n";

    //     std::cout << "PC: 0x"
    //               << std::hex
    //               << std::uppercase
    //               << std::setw(4)
    //               << std::setfill('0')
    //               << rom.get_pc();

    //     std::cout << "   Instruction: 0x"
    //               << std::setw(4)
    //               << instruction
    //               << '\n';

    //     // 2. DECODE
    //     DecodedInstruction decoded = decoder.decode(instruction);

    //     // 3. TEST 
    //     std::cout << std::dec;

    //     std::cout << "Opcode: "
    //               << static_cast<int>(decoded.opcode)
    //               << '\n';

    //     if (decoded.format == Format::R)
    //     {
    //         std::cout << "Format: R-Type\n";

    //         std::cout << "rd:  R"
    //                   << static_cast<int>(decoded.rd)
    //                   << '\n';

    //         std::cout << "rs1: R"
    //                   << static_cast<int>(decoded.rs1)
    //                   << '\n';

    //         std::cout << "rs2: R"
    //                   << static_cast<int>(decoded.rs2)
    //                   << '\n';
    //     }

    //     else if (decoded.format == Format::I)
    //     {
    //         std::cout << "Format: I-Type\n";

    //         if (decoded.opcode == Opcode::SW)
    //         {
    //             std::cout << "rs2: R"
    //                       << static_cast<int>(decoded.rs2)
    //                       << '\n';
    //         }
    //         else
    //         {
    //             std::cout << "rd:  R"
    //                       << static_cast<int>(decoded.rd)
    //                       << '\n';
    //         }

    //         std::cout << "rs1: R"
    //                   << static_cast<int>(decoded.rs1)
    //                   << '\n';

    //         std::cout << "Immediate: "
    //                   << decoded.immediate
    //                   << '\n';
    //     }

    //     else if (decoded.format == Format::C)
    //     {
    //         std::cout << "Format: C-Type\n";

    //         if (decoded.opcode == Opcode::BEQ)
    //         {
    //             std::cout << "rs1: R"
    //                       << static_cast<int>(decoded.rs1)
    //                       << '\n';

    //             std::cout << "rs2: R"
    //                       << static_cast<int>(decoded.rs2)
    //                       << '\n';
    //         }
    //         else
    //         {
    //             std::cout << "rd: R"
    //                       << static_cast<int>(decoded.rd)
    //                       << '\n';
    //         }

    //         std::cout << "Offset: "
    //                   << decoded.immediate
    //                   << '\n';
    //     }

    //     // Move to next instruction
    //     rom.next_instr();
    // }


/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////Test 3 Fetch -> Decode > disassembler  ///////////////////////////////////////////////////
    *////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Disassembler disassembler;

    // while (rom.get_pc() + 1 < program.size())
    // {
    //     // 1. Fetch
    //     u32 current_pc = rom.get_pc();
    //     u16 instruction = rom.fetch16();

    //     //2. Decode
    //     DecodedInstruction decoded =
    //         decoder.decode(instruction);

    //     //3. disassembly
    //     std::string assembly_text =
    //         disassembler.disassemble(decoded);

    //     //4. Test outitt
    //     std::cout << assembly_text << '\n';

    //     rom.next_instr();
    // }

    /*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////Test 3 Fetch -> Decode > execute  ///////////////////////////////////////////////////
    *////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RegFile regs;
    while (rom.get_pc() + 1 < program.size())
    {
        // 1. Fetch
        u32 current_pc = rom.get_pc();
        u16 instruction = rom.fetch16();

        //2. Decode
        DecodedInstruction decoded =
            decoder.decode(instruction);


                /*////////////////////////////////////////////////////////////
        //////////////////// READ OPERANDS ////////////////////////////
        ////////////////////////////////////////////////////////////*/

        ReadyInstruction ready =
            decoder.read_operands(decoded, regs);


        /*////////////////////////////////////////////////////////////
        ////////////////////////// EXECUTE ////////////////////////////
        ////////////////////////////////////////////////////////////*/

        action result =
            ALU::execute(ready);


        /*////////////////////////////////////////////////////////////
        ////////////////////// WRITE BACK /////////////////////////////
        ////////////////////////////////////////////////////////////*/

        if (result.flag == 0)
        {
            regs.write(result.rd, result.result);
        }


        // /*////////////////////////////////////////////////////////////
        // ////////////////////// PRINT TEST /////////////////////////////
        // ////////////////////////////////////////////////////////////*/

        // std::cout << "PC: 0x"
        //           << std::hex
        //           << std::uppercase
        //           << std::setw(4)
        //           << std::setfill('0')
        //           << current_pc

        //           << "   Instruction: 0x"
        //           << std::setw(4)
        //           << instruction

               

        //           << '\n';


        ////////////////// PRINT REGISTER FILE /////////////////////////
        ////////////////////////////////////////////////////////////*/

        std::cout << "\nFinal register values:\n";

        for (int i = 0; i <= 7; ++i)
         {
            std::cout << "R"
                  << i
                  << " = "
                  << regs.read(i)
                  << '\n';
         }
        
      rom.next_instr();
    }
    return 0;
}
    


