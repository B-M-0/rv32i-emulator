#include "Core.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <program.bin>\n";
        return 1;
    }

    Core process;
    try {
        process.load(argv[1]);
        while (!process.is_halted())
            process.step();
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    process.dump_regs(std::cout);
    return 0;
}
