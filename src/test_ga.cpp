#include "ga/serial_ga.h"
#include <iostream>

int main() {
    SerialGA demo(1000, 100);
    demo.performSelection();
    Individual best = demo.GetBestIndividual();
    best.save("demo_1000_100");
    return 0;
}