#pragma once

#include "ga/base_ga.h"

class SerialGA: public BaseGA{
public:
    SerialGA(int populationSize = DEFAULT_POPULATION, int numSteps = DEFAULT_STEP, int thread_num = 1);
    
    void SelectionStep() override;
};