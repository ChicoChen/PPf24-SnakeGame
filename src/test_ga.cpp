#include "ga/serial_ga.h"
#include <iostream>

int main() {
    SerialGA demo(10, 5);
    demo.performSelection();
    Individual best = demo.GetBestIndividual();
    return 0;
}