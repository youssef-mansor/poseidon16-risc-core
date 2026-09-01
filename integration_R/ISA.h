#ifndef POSEIDON16_ISA_H
#define POSEIDON16_ISA_H

#include <cstdint>
#include <string>
#include <array>
#include <vector>
#include <stdexcept> // for throwing an error on bad input
#include <iostream>  


namespace poseidon16
{
    using u8  = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using i8  = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;

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
        ADD   = 0x0,
        SUB   = 0x1,
        AND   = 0x2,
        OR    = 0x3,
        SLT   = 0x4,
        MUL   = 0x5,
        DIV   = 0x6,
        REM   = 0x7,
 
        ADDI  = 0x8, 
        ANDI  = 0x9,
        ORI   = 0xA,
        //SLTI  = 0xB, // not defined in the document by eng. ramy so won't implement
        LOAD  = 0xC, // by reem 
        STORE = 0xD, // by reem

        BEQ   = 0xE,
        JAL   = 0xF
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

        std::int32_t immediate; // do we need it as 32???? R
    };
    // decode does 2 things (decode + reg read)
    struct ReadyInstruction // the actual output of the decode after reading the rs1, rs2 values from regfile
    {
        Opcode opcode;
        Format format;
        std::uint8_t rs1; // these are redundant might also remove them in future
        std::uint8_t rs2; // these are redundant might also remove them

        std::uint8_t rd;
        std::int32_t reg1_data;
        std::int32_t reg2_data;

        std::int32_t immediate; // do we need it as 32???? will check on it
    }; 

    // this struct is the output of the alu. It defines the actions needed to be done on data_mem or regfile
    struct action 
    {
       std::uint8_t rd; 
       std::int32_t result; // this will be the address in case of load or store 
      //std::int32_t  store_value; // this needs to be added later
       std::uint8_t  flag;  // indicate whether we write back in reg or load/store in 
       // 0 means write in reg, 1 load 2 store
    };


    struct RegFile; // forward declaration

    // Classes
    class Decoder
    {
    public:
        DecodedInstruction decode(std::uint16_t instruction);
        ReadyInstruction read_operands(const DecodedInstruction &decoded, const RegFile &regs); // added by reem
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


    ///..... content of alu.h to have centeralized header .......///
    class ALU
    {
    public:
        //static int32_t execute(Opcode opcode, int32_t a, int32_t b);
        static action execute(ReadyInstruction insta);
    };

    ///..... content of rom.h to have centeralized header .......///
    class ROM
    {
    public:
        ROM(const std::vector<uint8_t>& program);

        void next_instr();
        void update_pc(u32 offset);
        u32 get_pc()  const;
        u16 fetch16() const;

    private:
        std::array<u8, 64> memory{}; // maximum number of instructions is 32, 64 bytes
        u32 pc = 0;
    };


}

#endif // POSEIDON16_ALU_H
