#include "Core.h"
#include <iostream>

int main(int argc, char* argv[])
{
    bool trace = false;
    const char* path = nullptr;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--trace")
            trace = true;
        else
            path = argv[i];
    }
    if (!path) {
        std::cerr << "usage: " << argv[0] << " [--trace] <program.bin>\n";
        return 1;
    }

    Core process;
    if (trace)
        process.set_trace(&std::cout);   // one line per instruction, before the register dump
    try {
        process.load(path);
        while (!process.is_halted())
            process.step();
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    process.dump_regs(std::cout);
    return 0;
}
