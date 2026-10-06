#include "decode.h"
#include "Core.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <charconv>
#include <fstream>
#include <sstream>

static int total_failures = 0;



void test_imm_i(){
    int count = 0;
    std::cout << "testing (imm_i):\n";
    int test_count = 6;
    std::string failures = "";

    std::vector<uint32_t> test_cases = {
        0x00010093,
        0x00110093,
        0xFFF10093,
        0x7FF10093,
        0x80010093,
        0xFF812083
    };
    std::vector<int32_t> expected = { 0, 1, -1, 2047, -2048,  -8};
    for(int i = 0 ; i < test_count;i++)
    {
        int32_t case_i = imm_i(test_cases[i]);
        if (case_i != expected[i]) {
            count++;
            total_failures++;
            failures = failures + std::to_string(i) + "  ";
        }
    }
    if (count == 0)
        std::cout << "All tests passed!\n";
    else
    {
        std::cout << (test_count - count) << " of " << test_count <<" tests passed\n";
        std::cout << "Failure cases: " << failures << "\n";
    }
}

void test_imm_s(){
    int count = 0;
    std::cout << "testing (imm_s):\n";
    int test_count = 5; 
    std::string failures = "";

    std::vector<uint32_t> test_cases = {
        0x00542023,
        0x00542223,
        0xFE542E23,
        0x7E542FA3,
        0x80542023
    };
    std::vector<int32_t> expected = { 0, 4, -4, 2047, -2048};
    for(int i = 0 ; i < test_count;i++)
    {
        int32_t case_i = imm_s(test_cases[i]);
        if (case_i != expected[i]) {
            count++;
            total_failures++;
            failures = failures + std::to_string(i) + "  ";
        }
    }
    if (count == 0)
        std::cout << "All tests passed!\n";
    else
    {
        std::cout << (test_count - count) << " of " << test_count <<" tests passed\n";
        std::cout << "Failure cases: " << failures << "\n";
    }
}

void test_imm_j(){
    int count = 0;
    std::cout << "testing (imm_j):\n";
    int test_count = 4; 
    std::string failures = "";

    std::vector<uint32_t> test_cases = {
        0x000000EF,
        0x004000EF,
        0xFFDFF0EF,
        0x000010EF
    };
    std::vector<int32_t> expected = {0, 4, -4, 4096};
    for(int i = 0 ; i < test_count;i++)
    {
        int32_t case_i = imm_j(test_cases[i]);
        if (case_i != expected[i]) {
            count++;
            total_failures++;
            failures = failures + std::to_string(i) + "  ";
        }
    }
    if (count == 0)
        std::cout << "All tests passed!\n";
    else
    {
        std::cout << (test_count - count) << " of " << test_count <<" tests passed\n";
        std::cout << "Failure cases: " << failures << "\n";
    }
}


void test_imm_b(){
    int count = 0;
    std::cout << "testing (imm_b):\n";
    int test_count = 6;
    std::string failures = "";

    // beq, bne, blt, bge, bltu, bgeu from test.s (B-type section)
    std::vector<uint32_t> test_cases = {
        0x00208063,
        0x00209263,
        0x0220C263,
        0xFC20DAE3,
        0xFE20EEE3,
        0xFC20F6E3
    };
    std::vector<int32_t> expected = {0, 4, 36, -44, -4, -52};
    for(int i = 0 ; i < test_count;i++)
    {
        int32_t case_i = imm_b(test_cases[i]);
        if (case_i != expected[i]) {
            count++;
            total_failures++;
            failures = failures + std::to_string(i) + "  ";
        }
    }
    if (count == 0)
        std::cout << "All tests passed!\n";
    else
    {
        std::cout << (test_count - count) << " of " << test_count <<" tests passed\n";
        std::cout << "Failure cases: " << failures << "\n";
    }
}

void test_imm_u(){
    int count = 0;
    std::cout << "testing (imm_u):\n";
    int test_count = 5;
    std::string failures = "";

    // lui x1,0 / lui x1,1 / lui x1,0x12345 / lui x1,0xFFFFF / auipc x1,0x1000 from test.s (U-type section)
    std::vector<uint32_t> test_cases = {
        0x000000B7,
        0x000010B7,
        0x123450B7,
        0xFFFFF0B7,
        0x01000097
    };
    std::vector<int32_t> expected = {0, 4096, 305418240, -4096, 16777216};
    for(int i = 0 ; i < test_count;i++)
    {
        int32_t case_i = imm_u(test_cases[i]);
        if (case_i != expected[i]) {
            count++;
            total_failures++;
            failures = failures + std::to_string(i) + "  ";
        }
    }
    if (count == 0)
        std::cout << "All tests passed!\n";
    else
    {
        std::cout << (test_count - count) << " of " << test_count <<" tests passed\n";
        std::cout << "Failure cases: " << failures << "\n";
    }
}


struct test_case{
    uint32_t input;
    int32_t expected;
};

template <typename Fn>

void test_extractor_func(std::string function_name,
                        Fn f,
                        const std::vector<test_case>& cases){
    int count = 0;
    std::cout << "testing (" << function_name << "):\n";
    int test_count = cases.size();
    std::string failures = "";

    for(int i = 0 ; i < test_count;i++)
    {
        auto case_i = f(cases[i].input);
        if (case_i != cases[i].expected) {
            count++;
            total_failures++;
            failures = failures + std::to_string(i) + "  ";
        }
    }
    if (count == 0)
        std::cout << "All tests passed!\n";
    else
    {
        std::cout << (test_count - count) << " of " << test_count <<" tests passed\n";
        std::cout << "Failure cases: " << failures << "\n";
    }
}

void check_vectors(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) { std::cerr << "could not open " << filename << '\n'; total_failures++; return; }
    int counter= 0;
    int total = 0;
    std::string line;
    while (std::getline(file, line)) {
       
        std::istringstream ss(line);
        std::string addr, word_str, mnemonic;
        if (!(ss >> addr >> word_str >> mnemonic)) continue;

        uint32_t word{};
        const char* end = word_str.data() + word_str.size();
        auto [ptr, ec] = std::from_chars(word_str.data(), end, word, 16);
        if (ec != std::errc{} || ptr != end) continue;

        std::string expected = op_names[static_cast<std::size_t>(op_of(word, opcode_of(word)))];
        total++;
        if (expected != mnemonic) {
            std::cout << expected<< '\t' << mnemonic << '\n';
            total_failures++;
        }
        else
            counter++;
    }

    if (counter == total)
        std::cout << "All tests passed!\n";
    else
        std::cout  << counter << '\\' << total << " tests passed\n";
}


struct reg_case{
    uint32_t reg;
    uint32_t expected;
};

// Runs a test program until it halts, then checks the given registers.
// Expected values come from the comments in the program's .s file.
void test_program(const std::string& path, const std::vector<reg_case>& cases){
    std::cout << "testing (" << path << "):\n";

    Core core;
    core.load(path);
    int steps = 0;
    while (!core.is_halted() && steps < 10000) {   // limit catches infinite loops
        core.step();
        steps++;
    }
    if (!core.is_halted()) {
        std::cout << "did not halt within " << steps << " steps\n";
        total_failures++;
        return;
    }

    int count = 0;
    for (const auto& c : cases) {
        uint32_t got = core.read_R(c.reg);
        if (got != c.expected) {
            count++;
            total_failures++;
            std::cout << "  x" << std::dec << c.reg << ": got 0x" << std::hex << got
                      << ", expected 0x" << c.expected << std::dec << "\n";
        }
    }
    if (count == 0)
        std::cout << "All tests passed!\n";
    else
        std::cout << (cases.size() - count) << " of " << cases.size() << " tests passed\n";
}


int main(){
    test_imm_i();
    test_imm_s();
    test_imm_j();
    test_imm_b();
    test_imm_u();

    test_extractor_func("imm_i", imm_i, {
        {0x00010093, 0},
        {0x00110093, 1},
        {0xFFF10093,-1},
        {0x7FF10093,2047},
        {0x80010093,-2048},
        {0xFF812083,-8}
    });

    // raw words below are reused from the imm_* tests above (addi -1, sw +4,
    // bne +4, lui 0x12345, jal +4) — one instruction per format, so each
    // field extractor gets a mix of zero and non-zero bit patterns to check.
    // For the I/U/J-format words, rs2/funct3/funct7 aren't meaningful RISC-V
    // fields (those bits are immediate bits for those formats); the test
    // only checks the raw bit extraction is correct, not ISA semantics.
    std::vector<test_case> opcode_cases = {
        {0xFFF10093, 0x13},
        {0x00542223, 0x23},
        {0x00209263, 0x63},
        {0x123450B7, 0x37},
        {0x004000EF, 0x6F}
    };
    test_extractor_func("opcode", opcode_of, opcode_cases);

    std::vector<test_case> rd_cases = {
        {0xFFF10093, 1},
        {0x00542223, 4},
        {0x00209263, 4},
        {0x123450B7, 1},
        {0x004000EF, 1}
    };
    test_extractor_func("rd", rd_of, rd_cases);

    std::vector<test_case> rs1_cases = {
        {0xFFF10093, 2},
        {0x00542223, 8},
        {0x00209263, 1},
        {0x123450B7, 8},
        {0x004000EF, 0}
    };
    test_extractor_func("rs1", rs1_of, rs1_cases);

    std::vector<test_case> rs2_cases = {
        {0xFFF10093, 31},
        {0x00542223, 5},
        {0x00209263, 2},
        {0x123450B7, 3},
        {0x004000EF, 4}
    };
    test_extractor_func("rs2", rs2_of, rs2_cases);

    std::vector<test_case> funct3_cases = {
        {0xFFF10093, 0},
        {0x00542223, 2},
        {0x00209263, 1},
        {0x123450B7, 5},
        {0x004000EF, 0}
    };
    test_extractor_func("funct3", funct3_of, funct3_cases);

    std::vector<test_case> funct7_cases = {
        {0xFFF10093, 127},
        {0x00542223, 0},
        {0x00209263, 0},
        {0x123450B7, 9},
        {0x004000EF, 0}
    };
    test_extractor_func("funct7", funct7_of, funct7_cases);

    check_vectors("rv32all_dump.txt");

    // register numbers: a0-a7 = x10-x17, s2-s8 = x18-x24
    test_program("tests/programs/alu_r.bin", {
        {10, 0x80000000},   // add  overflow
        {11, 11},           // sub
        {12, 0xFFFFFFFE},   // sub  negative result
        {13, 0x7FFFFFF8},   // and
        {14, 0xFFFFFFFB},   // or
        {15, 0x80000007},   // xor
        {16, 2},            // sll  shift count masked to 5 bits
        {17, 0xFFFFFFF8},   // sll
        {18, 0x1FFFFFFF},   // srl
        {19, 0xFFFFFFFF},   // sra  negative
        {20, 0x0FFFFFFF},   // sra  positive
        {21, 1},            // slt  signed
        {22, 0},            // slt
        {23, 0},            // sltu unsigned
        {24, 1},            // sltu
        {0,  0},            // x0 never changes
    });

    // a0-a7 = x10-x17, s2-s10 = x18-x26
    test_program("tests/programs/alu_i.bin", {
        {10, 4},            // addi negative imm
        {11, 0x804},        // addi largest imm
        {12, 0xFFFFF800},   // addi smallest imm, sign-extended
        {13, 0x80000000},   // addi overflow
        {14, 1},            // slti signed
        {15, 0},            // slti
        {16, 1},            // sltiu imm sign-extended then unsigned
        {17, 1},            // sltiu seqz
        {18, 0},            // sltiu equal
        {19, 0xFFFFFFFA},   // xori -1 (not)
        {20, 0xFFFFF805},   // ori
        {21, 0x000007F0},   // andi
        {22, 0x7FFFF800},   // andi negative imm
        {23, 0x80000000},   // slli
        {24, 0x0000000F},   // srli
        {25, 0xFFFFFFFC},   // srai negative
        {26, 1},            // srai positive
    });

    // a0-a7 = x10-x17, s2-s4 = x18-x20
    test_program("tests/programs/load_store.bin", {
        {10, 0x8081FF7F},   // lw
        {11, 0x0000007F},   // lb  positive
        {12, 0xFFFFFFFF},   // lb  sign-extended
        {13, 0x000000FF},   // lbu zero-extended
        {14, 0xFFFF8081},   // lh  sign-extended
        {15, 0x00008081},   // lhu zero-extended
        {16, 0xFFFFFF7F},   // lh
        {17, 0x8081FF7F},   // lw  negative offset
        {18, 0x5678FF78},   // sb + sh only change their own bytes
        {19, 0x00000000},   // next word untouched
        {20, 0x0081FF7F},   // sb zero
    });

    // 1 = taken, 0 = not taken. a0-a7 = x10-x17, s2-s8 = x18-x24
    test_program("tests/programs/branch.bin", {
        {10, 1},            // beq  taken
        {11, 0},            // beq  not taken
        {12, 1},            // bne  taken
        {13, 0},            // bne  not taken
        {14, 1},            // blt  taken (signed)
        {15, 0},            // blt  not taken
        {16, 1},            // bge  taken
        {17, 1},            // bge  equal
        {18, 0},            // bge  not taken
        {19, 1},            // bltu taken (unsigned)
        {20, 0},            // bltu not taken
        {21, 1},            // bgeu taken
        {22, 0},            // bgeu not taken
        {23, 1},            // bgeu equal
        {24, 5},            // backward branch loop count
    });

    // a0-a7 = x10-x17, s2-s4 = x18-x20
    test_program("tests/programs/jump.bin", {
        {10, 0},            // jal skipped the instruction
        {11, 4},            // jal link = pc + 4
        {12, 0},            // jalr skipped
        {13, 0},            // jalr skipped
        {14, 8},            // jalr link = pc + 4, target bit 0 cleared
        {15, 7},            // backward jal
        {16, 11},           // call and return
        {17, 0xFFFFF000},   // lui
        {20, 0x1004},       // auipc
    });

    if (total_failures != 0) {
        std::cout << total_failures << " failure(s)\n";
        return 1;
    }
    return 0;
}
