#pragma once
#include "decode.h"
#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <ostream>


class Core {
    private:
        static constexpr uint32_t MEM_BASE = 0x80000000;
        static constexpr uint32_t MEM_SIZE = 1u << 24;

        uint32_t x[32]{};
        uint32_t pc = MEM_BASE;

        bool halted = false;        
        std::vector<uint8_t> mem = std::vector<uint8_t>(MEM_SIZE);

        void execute(Instruction instr);
        void write_R(uint32_t idx, uint32_t value) ;
        void write_M(uint32_t address, uint32_t value, uint8_t width) ;

    public:
        Core();
        uint32_t read_R(uint32_t idx) const;
        uint32_t read_M(uint32_t address, uint8_t width) const;

        void load(const std::string& filePath);
        void step();

        bool is_halted() const;  

        void dump_regs(std::ostream& out) const;
};