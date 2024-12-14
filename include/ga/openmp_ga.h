#pragma once

#include <base_ga.h>

class OpenmpGA: public BaseGA{
    OpenmpGA(int population_size = DEFAULT_POPULATION, int num_steps = DEFAULT_STEP, int thread_num = 1);
    void evaluate_fitness(int iteration, int print_interval) override;
    virtual void update_population() override;
    void selection_step() override;
};  
