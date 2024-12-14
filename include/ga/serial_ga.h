#pragma once

#include "ga/base_ga.h"

class SerialGA: public BaseGA{
public:
    SerialGA(int population_size = DEFAULT_POPULATION, int num_steps = DEFAULT_STEP, int thread_num = 1);

private:
    void selection_step() override;
};