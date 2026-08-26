
#include "decode.h"

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
    // Field Extraction.
    uint32_t opcode(uint32_t raw){
        return raw & 0x7F ;
    }

    uint32_t rd(uint32_t raw){
        return (raw >> 7) & 0x1F;
    }

    uint32_t rs1(uint32_t raw){
        return (raw >> 15) & 0x1F;
    }

    uint32_t rs2(uint32_t raw){
        return (raw >> 20) & 0x1F;
    }

    uint32_t funct3(uint32_t raw){
        return (raw >> 12) & 0x7;
    }

    uint32_t funct7(uint32_t raw){
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
