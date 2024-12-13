#include "ga/serial_ga.h"

#include <iostream>
#include <algorithm>

SerialGA::SerialGA(int population_size, int num_steps, int thread_num):
            BaseGA(population_size, num_steps, thread_num){
                #ifdef DEBUG
                std::cout << "initialize new SerialGA" << std::endl;
                #endif
            }

void SerialGA::selection_step() {
    std::sort(get_population().begin(), get_population().end(),
              [](Individual &a, Individual &b)-> bool {return a.get_fitness() > b.get_fitness();});
    current_population = num_survivor;
}