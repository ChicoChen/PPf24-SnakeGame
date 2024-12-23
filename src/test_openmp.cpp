#include <cstdlib>
#include <iostream>

#include "ga/openmp_ga.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <output_file> <thread_num>" << std::endl;
        return 1;
    }
    int thread_num = atoi(argv[2]);
    OpenmpGA demo(argv[1], 1024, 1500, thread_num);
    demo.execute(50);
    return 0;
}
