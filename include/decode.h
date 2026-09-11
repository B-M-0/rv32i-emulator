#include <cstdint>
#include <iostream>
#include <iterator>

enum class Format { R, I, S, B, U, J , INVALID};
enum class Op {INVALID,// unknown/invalid 
    LUI, AUIPC, // U-Type
    JAL, //J-Type
    JALR,//I-Type Jump
    BEQ,BNE,BLT,BGE,BLTU,BGEU, // B-type.
    LB,LH,LW,LBU,LHU, // I-Type - load
    SB, SH, SW, // S-Type - store 
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, // I-Type arithmetic
    SLLI, SRLI, SRAI,// I-Typee shift
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND, //  R-tpye
    FENCE, ECALL, EBREAK, // misc
    COUNT
};

inline const char* op_names[] = {
    "(invalid)",
    "lui", "auipc",
    "jal",
    "jalr",
    "beq", "bne", "blt", "bge", "bltu", "bgeu",
    "lb", "lh", "lw", "lbu", "lhu",
    "sb", "sh", "sw",
    "addi", "slti", "sltiu", "xori", "ori", "andi",
    "slli", "srli", "srai",
    "add", "sub", "sll", "slt", "sltu", "xor", "srl", "sra", "or", "and",
    "fence", "ecall", "ebreak"
};

static_assert(std::size(op_names) == static_cast<size_t>(Op::COUNT));

struct  Instruction{
    Format fmt;
    Op operation;

    uint32_t funct3;
    uint32_t funct7;

    int32_t imm;

    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;

};

Instruction decodeInstr(uint32_t raw);

Format format_of(uint32_t raw);
Op op_of(uint32_t raw, uint32_t opcode);

uint32_t opcode_of(uint32_t raw);
uint32_t rd_of(uint32_t raw);
uint32_t rs1_of(uint32_t raw);
uint32_t rs2_of(uint32_t raw);
uint32_t funct3_of(uint32_t raw);
uint32_t funct7_of(uint32_t raw);

int32_t imm_s(uint32_t raw);
int32_t imm_i(uint32_t raw);
int32_t imm_u(uint32_t raw);
int32_t imm_j(uint32_t raw);
int32_t imm_b(uint32_t raw);
