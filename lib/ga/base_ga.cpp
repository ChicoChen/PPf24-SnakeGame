#include "ga/base_ga.h"

#include <iostream>
#include <omp.h>
#include <cassert>
#include <algorithm>

const Individual& BaseGA::get_best_individual(){
    int best_indv_idx = -1;
    double max_score = 0;
    for(int i = 0; i < population_size; i++){
        double score = population[i].get_fitness();
        if(score > max_score){
            max_score = score;
            best_indv_idx = i;
        }
    }
    
    assert(best_indv_idx >= 0);
    return population[best_indv_idx];
}

void BaseGA::perform_selection(){
    for(int i = 0; i < num_steps; i++){
        #ifdef DEBUG
        std::cout << "--------------------------------" << std::endl;
        std::cout << "[baseGA] begin selection round: " << i << std::endl;
        #endif
        evaluate_fitness(i);
        selection_step();
        update_population();
    }
}

//TODO: build an env file to handle hyperparameters 
BaseGA::BaseGA(int population_size, int num_steps, int thread_num):
    population(population_size),
    population_size(population_size),
    current_population(population_size),
    best_scores(num_steps, 0),
    avg_scores(num_steps, 0),
    num_steps(num_steps),
    num_survivor(population_size * SURVIVAL_RATE),
    thread_num(thread_num){
        // construct one generator for each thread. default is serial
        std::random_device rd;
        for(int i = 0; i < thread_num; i++){
            genrators.emplace_back(rd());
        }
    }

//when Individual.fitness() is called, fitness score of each object is calculated and cached for furture access
void BaseGA::evaluate_fitness(int iteration){
    //* potential chance for parallel.
    for(int i = 0; i < population_size; i++){
        double fitness = population[i].calculate_fitness(genrators[0]);

        #ifdef DEBUG
        // TODO: another cmake macro for fine-grained output messages like this
        // std::cout << "[baseGA] evaluating agent: " << i
        //           << "\tscore: " << fitness << std::endl;
        #endif
        avg_scores[iteration] += fitness;
        best_scores[iteration] = (fitness > best_scores[iteration])?
                                fitness:
                                best_scores[iteration];
    }
    avg_scores[iteration] /= population_size;

    #ifdef DEBUG
    std::cout << "[baseGA] best_scores: " << best_scores[iteration] <<
                 ", avg_scores: " << avg_scores[iteration] << std::endl;
    #endif
}


void BaseGA::update_population(){
    double sum = 0;
    for(int i = 0; i < num_survivor; i++){
        sum += population[i].get_fitness();
    }
    
    // shuffle survivors to randomize parent slection
    std::shuffle(population.begin(), population.begin() + num_survivor, genrators[0]);
    while(current_population < population_size){
        std::vector<int> par = select_parents(sum);
        Individual &father = population[par[0]];
        Individual &mother = population[par[1]];

        for(auto &child : father.crossover(genrators[0], mother)){
            child.mutate(genrators[0]);
            population[current_population++] = std::move(child);
            //edge condition: only need to add one child 
            if(current_population == population_size) break;
        }
    }
}

std::vector<int> BaseGA::select_parents(double sum){
    assert(num_survivor >= 2 && sum >= 0);

    static int idx;
    std::vector<int> parents;
    std::uniform_real_distribution<> dis(0.0 , sum);

    for(int i = 0; i < 2; i++){
        double threshold = dis(genrators[0]);
        double count = 0;
        for(int candidate = 0; candidate < num_survivor; candidate++){
            count += population[candidate].get_fitness();
            if(count >= threshold){
                parents.push_back(candidate);
                break;
            }
        }
    }

    return parents;    
}