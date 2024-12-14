#include "ga/openmp_ga.h"

#include <iostream>

int main() {
    int thread_num = 6;
    OpenmpGA demo(1000, 1000, thread_num);

    demo.perform_selection(50);
    Individual best = demo.get_best_individual();
    best.save("openmp_1000_1000");

    demo.export_train_log("openmp_1000_1000.log", 50);
    return 0;
}