
#include "ISA.h"

namespace poseidon16
{

    action ALU :: execute(ReadyInstruction insta)
    {
        action execution_update{};

        execution_update.rd = insta.rd; 

        switch (insta.opcode)
        {   
            case Opcode::SW: // i need to calculate the address
             {
                execution_update.result = insta.reg1_data + insta.immediate;
                execution_update.flag   = 2;
                break; 
            } 
            
            case Opcode::LW: // i need to calculate the address 
            {
                execution_update.result = insta.reg1_data + insta.immediate;
                execution_update.flag   = 1;
                break; 
            }
            case Opcode::ADDI:
            {
                execution_update.result = insta.reg1_data + insta.immediate;
                execution_update.flag   = 0;
                break; 
            }
            case Opcode::ADD: 
            {
               execution_update.result = insta.reg1_data + insta.reg2_data;
               execution_update.flag   = 0;
               break; 
            }
            // case Opcode::SUBI:
            // {
            //     execution_update.result = insta.reg1_data - insta.immediate;
            //     execution_update.flag   = 0;
            //     break; 
            // }
            case Opcode::SUB:
            {
                execution_update.result = insta.reg1_data - insta.reg2_data;
                execution_update.flag = 0;
                break; 
            }
            case Opcode::ANDI:
            {
                execution_update.result = insta.reg1_data & insta.immediate;
                execution_update.flag   = 0;
                break; 
            }
            case Opcode::AND:
            {
                execution_update.result = insta.reg1_data & insta.reg2_data;
                execution_update.flag   = 0;
                break; 
            }
            case Opcode::ORI:
            {
                execution_update.result = insta.reg1_data | insta.immediate;
                execution_update.flag   = 0;
                break; 
            }
            case Opcode::OR:
            {
                execution_update.result = insta.reg1_data | insta.reg2_data;
                execution_update.flag   = 0;
                break; 
            }
            case Opcode::SLT:
            {
                execution_update.result = (insta.reg1_data < insta.reg2_data) ? 1 : 0;
                execution_update.flag   = 0;
                break; 
            }

            case Opcode::MUL: // added by reem
            {
                execution_update.result = insta.reg1_data * insta.reg2_data;
                execution_update.flag   = 0;
                break; 
            }
            case Opcode::DIV: 
            {
                execution_update.flag = 0;
                if (insta.reg2_data == 0)
                    execution_update.result = static_cast<int32_t>(DIV_BY_ZERO_RESULT); // added by reem
                else 
                    execution_update.result = insta.reg1_data / insta.reg2_data;
                
                break; 
            }
            case Opcode::REM:
            {   execution_update.flag = 0;
                if (insta.reg2_data == 0)
                    execution_update.result = insta.reg1_data; // spec: division by zero returns R[rs1] // added by reem
                else 
                    execution_update.result = insta.reg1_data % insta.reg2_data;
                break; 
            }
            default:
                throw std::invalid_argument("ALU::execute: opcode is not an ALU operation");
        }
    return execution_update; 

    }
};
