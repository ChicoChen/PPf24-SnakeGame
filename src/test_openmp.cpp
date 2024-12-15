#include "ga/openmp_ga.h"

int main() {
    int thread_num = 8;
    OpenmpGA demo("openmp_1024_1000", 1024, 1000, thread_num);
    demo.execute(50);
    return 0;
}