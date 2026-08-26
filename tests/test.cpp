#include "decode.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

void test_imm_i()
{
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

void test_imm_s()
{
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

void test_imm_j()
{
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
    std::vector<int32_t> expected = {0, 4, -4, 2048};
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

int main()
{
    //test_imm_i();
    //test_imm_s();
    //test_imm_j();
    
    std::cout << imm_j( 0x000010EF) << "\n";
    std::cout << imm_j( 0x004000ef) << "\n";
    std::cout << imm_j( 0x000000ef) << "\n";

    return 0;
}
