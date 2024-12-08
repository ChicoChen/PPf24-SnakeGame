#include "ga/serial_ga.h"
#include <iostream>

int main() {
    SerialGA demo;
    demo.performSelection();
    Individual best = demo.GetBestIndividual();
    return 0;
}