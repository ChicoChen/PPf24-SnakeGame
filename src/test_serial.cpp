#include "ga/serial_ga.h"

int main(int argc, char** argv) {
    SerialGA demo(argv[1], 1024, 1500);
    demo.execute(50);
    return 0;
}