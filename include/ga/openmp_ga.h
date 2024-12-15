#pragma once

#include "ga/base_ga.h"

class OpenmpGA : public BaseGA {
   public:
    OpenmpGA(std::string exp_name, int population_size = DEFAULT_POPULATION,
             int num_steps = DEFAULT_STEP, int thread_num = 1);

   private:
    void evaluate_fitness() override;
    void selection_step() override;
    void update_population() override;
};
