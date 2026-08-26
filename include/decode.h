#include <cstdint>
#include <iostream>

enum class Format { R, I, S, B, U, J , INVALID};

Format format_of(uint32_t raw);

int32_t imm_s(uint32_t raw);
int32_t imm_i(uint32_t raw);
int32_t imm_u(uint32_t raw);
int32_t imm_j(uint32_t raw);
int32_t imm_b(uint32_t raw)
