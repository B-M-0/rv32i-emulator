// Core.cpp
#include "Core.h"
#include "decode.h"


 
    Core::Core() {
        write_R(2u, MEM_BASE + MEM_SIZE);
    }

    void Core::execute(Instruction instr) {
        uint32_t next_pc = pc+4;
        Op op = instr.operation;
    
        uint32_t imm = instr.imm;
        uint32_t rd  = instr.rd;
        uint32_t rs1 = instr.rs1;
        uint32_t rs2 = instr.rs2;
        uint32_t a   = read_R(rs1);
        uint32_t b   = read_R(rs2);

        switch(op)
        {
            case Op::LUI:
                write_R(rd, imm);
                break;
            case Op::AUIPC:
                write_R(rd, pc + imm);
                break;
            case Op::JAL:
                write_R(rd, next_pc);
                pc = pc + imm;
                return;
            case Op::JALR:
                write_R(rd, next_pc);
                pc = (a + imm) & ~0b1;
                return;
            case Op::ADD:
                write_R(rd, a + b);
                break;
            case Op::SUB:
                write_R(rd, a - b);
                break;
            case Op::XOR:
                write_R(rd, a ^ b);
                break;
            case Op::OR:
                write_R(rd, a | b);
                break;
            case Op::AND:
                write_R(rd, a & b);
                break;
            case Op::SLL:
                write_R(rd, a << (b & 0b11111));
                break;
            case Op::SRL:
                write_R(rd, a >> (b & 0b11111));
                break;
            case Op::SRA:
                write_R(rd, (int32_t) a >> (b & 0b11111));
                break;
            case Op::SLT:
                write_R(rd, (int32_t) a < (int32_t) b ? 0b1 : 0b0);  
                break;
            case Op::SLTU:
                write_R(rd, a <  b ? 0b1 : 0b0);
                break;
            case Op::ADDI:
                write_R(rd, a + imm);
                break;
            case Op::XORI:
                write_R(rd, a ^ imm);
                break;
            case Op::ORI:
                write_R(rd, a | imm);
                break;
            case Op::ANDI:
                write_R(rd, a & imm);
                break;
            case Op::SLLI:
                write_R(rd, a << imm);
                break;
            case Op::SRLI:
                write_R(rd, a >> imm); 
                break;
            case Op::SRAI:
                write_R(rd, (int32_t) a >> imm);
                break;
            case Op::SLTI:
                write_R(rd, (int32_t) a < (int32_t) imm ? 0b1 : 0b0);
                break;
            case Op::SLTIU:
                write_R(rd, a < imm ? 0b1 : 0b0);
                break;
            case Op::LB:
                write_R(rd, (int32_t)((int8_t) read_M(imm + a, 1)));
                break;
            case Op::LH:
                write_R(rd, (int32_t)((int16_t) read_M(imm + a, 2)));
                break;
            case Op::LW:
                write_R(rd, read_M(imm + a, 4));
                break;
            case Op::LBU:
                write_R(rd, read_M(imm + a, 1));
                break;
            case Op::LHU:
                write_R(rd, read_M(imm + a, 2));
                break;
            case Op::SB:
                write_M(imm + a, (int8_t) b,1);
                break;
            case Op::SH:
                write_M(imm + a, (int16_t) b,2);
                break;
            case Op::SW:
                write_M(imm + a, b,4);
                break;
            // Branches: taken -> pc + imm, not taken -> fall through to pc + 4.
            case Op::BEQ:
                if (a == b) next_pc = pc + imm;
                break;
            case Op::BNE:
                if (a != b) next_pc = pc + imm;
                break;
            case Op::BLT:
                if ((int32_t) a <  (int32_t) b) next_pc = pc + imm;
                break;
            case Op::BGE:
                if ((int32_t) a >= (int32_t) b) next_pc = pc + imm;
                break;
            case Op::BLTU:
                if (a <  b) next_pc = pc + imm;
                break;
            case Op::BGEU:
                if (a >= b) next_pc = pc + imm;
                break;
            case Op::FENCE:
                break;
            case Op::ECALL:
                halted = true;
                break;
            case Op::EBREAK:
                halted = true;
                break;
            case Op::INVALID:
                halted = true;
                std::cerr << std::hex << instr.raw <<  " : " << pc << std::dec << ";  INVALID INSTRUCTION, the decoder didn't recognise this instruction."<< "\n";
                break;
            default:
                halted = true;
                std::cerr << std::hex << instr.raw <<  " : " << pc << std::dec << ";  UNRECOGNISED OPERATOR, the decoder recognised this instruction, it may not have been implemented in execute yet " <<"\n";
                break;


        }
        pc = next_pc;
    }

    void Core::step() {
        execute(decodeInstr(read_M(pc,4)));
    }


    bool Core::is_halted() const { 
        return halted; 
    }
    
        // register access 
    uint32_t Core::read_R(uint32_t idx) const {
        return idx != 0u ?  x[idx] :   0u;
    }
    void Core::write_R(uint32_t idx, uint32_t value) {
        if(idx != 0u) x[idx] = value;
    }   

    // memory access
    // width is 1, 2 or 4 bytes, little-endian. Result is zero-extended;
    // LB/LH must sign-extend it themselves.
    uint32_t Core::read_M(uint32_t address, uint8_t width) const {
        uint32_t tmp = address-MEM_BASE;
        if ( tmp > MEM_SIZE - width)
            throw std::out_of_range("read_M: address out of range: " + std::to_string(address));

        uint32_t value = 0;
        for(uint32_t i = 0 ; i < width; i++)
            value |= (uint32_t)mem[tmp + i] << (8*i);
        return value;
    }
    // Writes the low `width` bytes of value.
    void Core::write_M(uint32_t address, uint32_t value, uint8_t width) {
        uint32_t tmp = address-MEM_BASE;
        if ( tmp > MEM_SIZE - width)
            throw std::out_of_range("write_M: address out of range: " + std::to_string(address));

        for(uint32_t i = 0 ; i < width; i++)
            mem[tmp + i] = (uint8_t)((value >> (8*i)) & 0xFF);
    }


    void Core::load(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary);
        if (!file)
            throw std::runtime_error("load: could not open " + path);

        file.read(reinterpret_cast<char*>(mem.data()), MEM_SIZE);

        if (file.peek() != EOF)
            throw std::runtime_error("load: program bigger than memory");

    }
    
   

    void Core::dump_regs(std::ostream& out) const
    {
        static const char* abi[32] = {
            "zero","ra","sp","gp","tp","t0","t1","t2",
            "s0","s1","a0","a1","a2","a3","a4","a5",
            "a6","a7","s2","s3","s4","s5","s6","s7",
            "s8","s9","s10","s11","t3","t4","t5","t6"};

        out << "pc       = 0x" << std::hex << std::setw(8) << std::setfill('0') << pc << "\n";
        for (int i = 0; i < 32; i++)
            out << std::dec << "x" << std::setw(2) << std::setfill(' ') << i
                << " " << std::setw(4) << abi[i]
                << " = 0x" << std::hex << std::setw(8) << std::setfill('0') << x[i]
                << "  (" << std::dec << (int32_t)x[i] << ")\n";
    }






