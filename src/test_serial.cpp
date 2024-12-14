#include "ga/serial_ga.h"
#include <iostream>

int main() {
    SerialGA demo(1000, 1000);
    demo.perform_selection(50);
    Individual best = demo.get_best_individual();
    best.save("serial_1000_1000");

    demo.export_train_log("serial_1000_1000.log", 50);
    return 0;
}