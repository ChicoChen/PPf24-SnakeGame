#include "ga/openmp_ga.h"

#include <iostream>
#include <algorithm>
#include <omp.h>

OpenmpGA::OpenmpGA(int population_size, int num_steps, int thread_num)
         :BaseGA(population_size, num_steps, thread_num)
{
    #ifdef DEBUG
    std::cout << "initialize new OpenmpGA" << std::endl;
    #endif
}

void OpenmpGA::selection_step(){
    std::sort(population.begin(), population.end(),
              [](Individual &a, Individual &b)-> bool {return a.get_fitness() > b.get_fitness();});
    current_population = num_survivor;
}

void OpenmpGA::evaluate_fitness(int iteration, int print_interval){

    #pragma omp parallel
    {
        double local_sum = 0;
        double local_best = 0;
        int thread_id = omp_get_thread_num();

        #pragma omp for
        for(int i = 0 ; i < population_size; i++){
            double fitness = population[i].calculate_fitness(genrators[thread_id].gen);
            local_sum += fitness;
            local_best = (fitness > local_best)? fitness: local_best;
        }

        #pragma omp critical
        {
            avg_scores[iteration] += local_sum;
            best_scores[iteration] = (local_best > best_scores[iteration])?
                                      local_best:
                                      best_scores[iteration];
        }
    }
    avg_scores[iteration] /= population_size;

    // print debug message
    #ifdef DEBUG
    if(iteration % print_interval == 0){
        std::cout << "[baseGA] best_scores: " << best_scores[iteration] <<
                     ", avg_scores: " << avg_scores[iteration] << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }
    #endif
}

void OpenmpGA::update_population(){
    double sum = 0;

    #pragma omp parallel for reduction(+, sum)
    for(int i = 0; i < num_survivor; i++){
        sum += population[i].get_fitness();
    }
    std::shuffle(population.begin(), population.begin() + num_survivor, genrators[0]);

    int target_num = population_size - current_population;
    int thread_workload = target_num / thread_num;
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        const int local_start_pos = current_population + thread_id * thread_workload;
        const int local_end_pos = (thread_id == thread_num - 1)?
                                   thread_num:
                                   current_population + (thread_id + 1) * thread_workload;
        
        std::vector<Individual> local_offsprings;
        for(int i = 0; local_start_pos + i < local_end_pos; i++){
            std::vector<int> par = select_parents(sum, genrators[thread_id].gen);
            Individual &father = population[par[0]];
            Individual &mother = population[par[1]];

            for(auto &child : father.crossover(genrators[thread_id].gen, mother)){
                child.mutate(genrators[thread_id].gen);
                local_offsprings[i++] = std::move(child);
                if(local_start_pos + i == local_end_pos) break;
            }
        }

        #pragma omp critical
        for(int i = local_start_pos; i < local_end_pos; i++){
            population[i] = std::move(local_offsprings[i - local_start_pos]);
        }
    }
}
