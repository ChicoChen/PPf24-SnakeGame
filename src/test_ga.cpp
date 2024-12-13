#include "ga/serial_ga.h"
#include <iostream>

int main() {
    SerialGA demo(1000, 1000);
    demo.perform_selection();
    Individual best = demo.get_best_individual();
    best.save("demo_1000_1000");
    return 0;
}