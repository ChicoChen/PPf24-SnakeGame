#include "ga/base_ga.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <chrono>

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

void BaseGA::perform_selection(int print_interval){
    start_time = chrono_clock::now();
    for(int i = 0; i < num_steps; i++){
        #ifdef DEBUG
        if (i % print_interval == 0){
            std::cout << "[baseGA] round: " << i << std::endl;
        }
        #endif

        evaluate_fitness(i, print_interval);
        selection_step();
        update_population();
    }

    total_time = chrono_clock::now() - start_time;
    #ifdef DEBUG
    std::cout << "[baseGA] total_selection time: " << total_time.count() << 's' << std::endl;

    #endif
}

// log the average and best performance during selection
void BaseGA::export_train_log(const std::string& logfile, int log_interval){
    std::ofstream log(logfile);
    if (!log.is_open()) {
        std::cerr << "[BaseGA]Failed to open logfile " << logfile << std::endl;
        return;
    }

    log << population_size << ' ' << num_steps << ' '
        << total_time.count() << std::endl;
    for(int i = 0; i < num_steps; i++){
        if(i % log_interval == 0)
            log  << i << ' ' << best_scores[i] << ' ' << avg_scores[i] << std::endl;
    }
    std::cout << "[BaseGA] exported log file " << logfile << std::endl;
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
            generators.emplace_back(rd());
        }
    }

//when Individual.fitness() is called, fitness score of each object is calculated and cached for furture access
void BaseGA::evaluate_fitness(int iteration, int print_interval){
    //* potential chance for parallel.
    for(int i = 0; i < population_size; i++){
        double fitness = population[i].calculate_fitness(generators[0].gen);
        avg_scores[iteration] += fitness;
        best_scores[iteration] = (fitness > best_scores[iteration])?
                                fitness:
                                best_scores[iteration];
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


void BaseGA::update_population(){
    double sum = 0;
    for(int i = 0; i < num_survivor; i++){
        sum += population[i].get_fitness();
    }
    
    // shuffle survivors to randomize parent slection
    std::shuffle(population.begin(), population.begin() + num_survivor, generators[0].gen);
    while(current_population < population_size){
        std::vector<int> par = select_parents(sum, generators[0].gen);
        Individual &father = population[par[0]];
        Individual &mother = population[par[1]];

        for(auto &child : father.crossover(generators[0].gen, mother)){
            child.mutate(generators[0].gen);
            population[current_population++] = std::move(child);
            //edge condition: only need to add one child 
            if(current_population == population_size) break;
        }
    }
}

std::vector<int> BaseGA::select_parents(double sum, std::mt19937 &generator){
    assert(num_survivor >= 2 && sum >= 0);

    static int idx;
    std::vector<int> parents;
    std::uniform_real_distribution<> dis(0.0 , sum);

    for(int i = 0; i < 2; i++){
        double threshold = dis(generator);
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