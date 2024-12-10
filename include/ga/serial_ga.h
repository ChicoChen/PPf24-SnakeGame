#pragma once

#include "ga/base_ga.h"

class SerialGA: public BaseGA{
public:
    SerialGA(int populationSize = DEFAULT_POPULATION, int numSteps = DEFAULT_STEP);
    
    void SelectionStep() override;
};