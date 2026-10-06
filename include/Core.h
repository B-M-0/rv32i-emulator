#pragma once
#include "decode.h"
#include <cstdint>
#include <cstdio>
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

        // What the current instruction changed, for the trace. Reset every step.
        struct TraceRecord {
            bool     reg_write = false;
            uint32_t rd = 0, rd_value = 0;
            bool     mem_read = false;
            uint32_t load_addr = 0;
            bool     mem_write = false;
            uint32_t store_addr = 0, store_value = 0;
            uint8_t  store_width = 0;
        };
        TraceRecord last{};
        std::ostream* trace_out = nullptr;   // nullptr = tracing off

        void execute(Instruction instr);
        void write_R(uint32_t idx, uint32_t value) ;
        void write_M(uint32_t address, uint32_t value, uint8_t width) ;
        uint32_t load_M(uint32_t address, uint8_t width);   // data read by a load instruction (traced)
        void print_trace(uint32_t at_pc, uint32_t raw) const;

    public:
        Core();
        uint32_t read_R(uint32_t idx) const;
        uint32_t read_M(uint32_t address, uint8_t width) const;

        void load(const std::string& filePath);
        void step();

        bool is_halted() const;
        void set_trace(std::ostream* out);   // pass nullptr to turn tracing off

        void dump_regs(std::ostream& out) const;
};