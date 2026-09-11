#include "decode.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <charconv>
#include <fstream>
#include <sstream>



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
    if (!file) { std::cerr << "could not open " << filename << '\n'; return; }
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
        if (expected != mnemonic)
            std::cout << expected<< '\t' << mnemonic << '\n';
        else
            counter++;
    }
    std::cout  << counter << '\\' << total << " tests passed\n";
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
    return 0;
}
