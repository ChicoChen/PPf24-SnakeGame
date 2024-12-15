#include "ga/serial_ga.h"

int main() {
    SerialGA demo("serial_1024_1000", 1024, 1000);
    demo.execute(50);
    return 0;
}