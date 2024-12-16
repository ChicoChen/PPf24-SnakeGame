#include "ga/openmp_ga.h"

int main(int argc , char** argv) {
    int thread_num = atoi(argv[2]);
    OpenmpGA demo(argv[1], 1024, 1500, thread_num);
    demo.execute(50);
    return 0;
}