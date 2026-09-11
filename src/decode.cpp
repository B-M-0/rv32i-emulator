
#include "decode.h"


    Instruction decode(uint32_t raw){
        uint32_t opcode = opcode_of(raw);
        Op operation = op_of(raw,opcode);
        Format fmt = format_of(opcode);
        switch(fmt){
        }
    }
    
    // Identifies the encoding variant form the opcode.  
    Format format_of(uint32_t opcode){
        switch (opcode){
            case 0x33:
                return Format::R;
            case 0x13:
                return Format::I;
            case 0x03:
                return Format::I;
            case 0x67:
                return Format::I;
            case 0x73:
                return Format::I;
            case 0x23:
                return Format::S;
            case 0x63:
                return Format::B;
            case 0x37:
                return Format::U; 
            case 0x17:
                return Format::U;
            case 0x6F:
                return Format::U;
            default:
                return Format::INVALID;
        }
    }
    
    // Extractions from raw.
    Op op_of(uint32_t raw, uint32_t opcode){
        uint32_t funct3 = funct3_of(raw);
        uint32_t funct7 = funct7_of(raw);
        switch (opcode){
            case 0b0110111:
                return Op::LUI;
            case 0b0010111:
                return Op::AUIPC;
            case 0b1101111:
                return Op::JAL;
            case 0b1100111:
                return funct3 == 0x0 ? Op::JALR : Op::INVALID;
            case 0b0110011:
                switch(funct3){
                    case 0x0:
                        switch (funct7) {
                            case 0x00:
                                return Op::ADD;
                            case 0x20:
                                return Op::SUB;
                            default:
                                return Op::INVALID;
                        }
                    case 0x4:
                        return funct7 == 0x00 ? Op::XOR : Op::INVALID;
                    case 0x6:
                        return funct7 == 0x00 ? Op::OR : Op::INVALID;
                    case 0x7:
                        return funct7 == 0x00 ? Op::AND : Op::INVALID;
                    case 0x1:
                        return funct7 == 0x00 ? Op::SLL : Op::INVALID;
                    case 0x5:
                        switch (funct7){
                            case 0x00:
                                return Op::SRL;
                            case 0x20:
                                return Op::SRA;
                            default: 
                                return Op::INVALID;
                        }
                    case 0x2:
                        return funct7 == 0x00 ? Op::SLT : Op::INVALID;
                    case 0x3:
                        return funct7 == 0x00 ? Op::SLTU : Op::INVALID;
                    default:
                        return Op::INVALID;
                }
            case 0b0010011:
                switch(funct3){
                    case 0x0:
                        return Op::ADDI;
                    case 0x4:
                        return Op::XORI;
                    case 0x6:
                        return Op::ORI;
                    case 0x7: 
                        return Op::ANDI;
                    case 0x1:
                        return funct7 == 0x00 ? Op::SLLI : Op::INVALID;
                    case 0x5:
                        switch(funct7){
                            case 0x00:
                                return Op::SRLI;
                            case 0x20:
                                return Op::SRAI;
                            default:
                                return Op::INVALID;
                        }
                    case 0x2:
                        return Op::SLTI;
                    case 0x3:
                        return Op::SLTIU;
                    default:
                        return Op::INVALID;
                }
            case 0b0000011:
                switch (funct3){
                    case 0x0:
                        return Op::LB;
                    case 0x1:
                        return Op::LH;
                    case 0x2:
                        return Op::LW;
                    case 0x4:
                        return Op::LBU;
                    case 0x5:
                        return Op::LHU;
                    default:
                        return Op::INVALID;
                }
            case 0b0100011:
                switch (funct3){
                    case 0x0:
                        return Op::SB;
                    case 0x1:
                        return Op::SH;
                    case 0x2:
                        return Op::SW;
                    default:
                        return Op::INVALID;
                }
            case 0b1100011:
                switch(funct3){
                    case 0x0:
                        return Op::BEQ;
                    case 0x1:
                        return Op::BNE;
                    case 0x4:
                        return Op::BLT;
                    case 0x5:
                        return Op::BGE; 
                    case 0x6:
                        return Op::BLTU;
                    case 0x7:
                        return Op::BGEU;
                    default:
                        return Op::INVALID;
                }
            default:
                return Op::INVALID;
        }

    }

    // Field Extraction.
    uint32_t opcode_of(uint32_t raw){
        return raw & 0x7F ;
    }

    uint32_t rd_of(uint32_t raw){
        return (raw >> 7) & 0x1F;
    }

    uint32_t rs1_of(uint32_t raw){
        return (raw >> 15) & 0x1F;
    }

    uint32_t rs2_of(uint32_t raw){
        return (raw >> 20) & 0x1F;
    }

    uint32_t funct3_of(uint32_t raw){
        return (raw >> 12) & 0x7;
    }

    uint32_t funct7_of(uint32_t raw){
        return (raw >> 25) & 0x7f;
    }

    // Immediate extraction.
    int32_t imm_i(uint32_t raw){
        return (int32_t) raw >> (20);
    }

    int32_t imm_s(uint32_t raw){
        uint32_t hi = ( raw >> (25)) & 0x7F ;
        uint32_t lo = ( raw >> (7)) & 0x1F  ;
        int32_t  out  = (int32_t)(((hi << 5) | lo) << 20) >> 20;
        return out;
    }

    int32_t imm_b(uint32_t raw){
        uint32_t imm_12   = ((raw >> 31) & 0x1)  << 12;
        uint32_t imm_11   = ((raw >>  7) & 0x1)  << 11;
        uint32_t imm_10_5 = ((raw >> 25) & 0x3F) << 5;
        uint32_t imm_4_1  = ((raw >>  8) & 0xF)  << 1;
        return ((int32_t)( (imm_12 | imm_11| imm_10_5 | imm_4_1) << 19)) >> 19;
    }

    int32_t imm_u(uint32_t raw){
        return (int32_t) raw & 0xFFFFF000;
    }

    int32_t imm_j(uint32_t raw){
        uint32_t imm_20    = ((raw >> 31) & 0x1) << 20;
        uint32_t imm_19_12 =   raw & 0xFF000;
        uint32_t imm_11    = ((raw >> 20) & 0x1) << 11;
        uint32_t imm_10_1  = ((raw >> 21) & 0x3FF) << 1;
        return ((int32_t)( (imm_20 | imm_19_12| imm_11 | imm_10_1) << 11)) >> 11;
    }
