// Core.cpp
#include "Core.h"

    
 
    // register access 
    uint32_t Core::read_R(uint32_t address) const {
        return address != 0u ?  x[address] :   0u;
    }
    void Core::write_R(uint32_t address, uint32_t value) {
        if(address != 0u) x[address] = value;
    }   

    // memory access
    uint32_t Core::read_M(uint32_t address) const {
        uint32_t tmp = address-MEM_BASE;
        if ( tmp <= mem_size - 4){
            uint32_t b0 = mem[tmp], b1 = mem[tmp + 1], b2 = mem[tmp + 2], b3 = mem[tmp + 3];  
            return b0 | (b1 << 8) | (b2 << 16) | (b3 << 24);
        }
        else
            throw exception();
    }
    void Core::write_M(uint32_t address, uint32_t value) {
        uint32_t tmp = address-MEM_BASE;
        if ( tmp <= mem_size -4) {
           for(int i = 0 ; i < 4; i++)
            {
                uint8_t byte = (uint8_t)((value >> i*8) & 0xFF);
                mem[tmp + i] = byte;
            }
        }
    }   


 
    
    void Core::step() {
        uint32_t instr = mem[pc - MEM_BASE ]
    }





