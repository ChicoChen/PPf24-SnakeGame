#include <cstdlib>
#include <iostream>

#include "ga/serial_ga.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output_file>" << std::endl;
    }
    SerialGA demo(argv[1], 1024, 1500);
    demo.execute(50);
    return 0;
}
