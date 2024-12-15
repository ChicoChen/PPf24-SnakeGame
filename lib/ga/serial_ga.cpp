#include "ga/serial_ga.h"

#include <algorithm>
#include <iostream>

SerialGA::SerialGA(std::string exp_name, int population_size, int num_steps, int thread_num)
    : BaseGA(exp_name, population_size, num_steps, thread_num) {
#ifdef DEBUG
    std::cout << "initialize new SerialGA" << std::endl;
#endif
}

void SerialGA::evaluate_fitness() {
    for (int i = 0; i < population_size; i++) {
        population[i].evaluate(generators[0].gen);
    }
}

void SerialGA::selection_step() {
    std::sort(population.begin(), population.end(),
              [](Individual &a, Individual &b) -> bool { return a.fitness > b.fitness; });
    current_population = num_survivor;
}
