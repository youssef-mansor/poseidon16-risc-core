#include "ISA.h"

namespace poseidon16
{

    void take_action(action last_action, RegFile &regs, Data_mem &mem) {

        if (last_action.flag == 0) // write in regfile
            regs.write(last_action.rd, last_action.result);
            
        else if (last_action.flag == 1) // load from memory 
            regs.write(last_action.rd, mem.load(last_action.result) ); //load + write back same step 

        else if (last_action.flag == 2) // store in memory
            mem.store(last_action.result, last_action.store_value);

    }
}