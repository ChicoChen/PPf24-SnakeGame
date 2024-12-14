#pragma once

#include "ga/individual.h"
#include <random>
#include <vector>
#include <string>
#include <chrono>

#define DEFAULT_POPULATION 100
#define DEFAULT_STEP 5
#define SURVIVAL_RATE 0.5
#define DEFAULT_TIME_INTERVAL 10

using chrono_clock = std::chrono::high_resolution_clock;
/*
Base class for all genetic algorithm implementations.
This class serves as the foundation for implementing various genetic algorithms. 
It provides common methods such as ...
*/
class BaseGA{
public:
    // return the best individual, parallelized by omp
    virtual const Individual& get_best_individual();
    
    // perform the whole selection process.
    void perform_selection(int print_interval = DEFAULT_TIME_INTERVAL);

    // export a log file for training info.
    void export_train_log(const std::string& filename, int log_interval = DEFAULT_TIME_INTERVAL);

    BaseGA(int population_size = DEFAULT_POPULATION, int num_steps = DEFAULT_STEP, int thread_num = 1);
    ~BaseGA() = default;

protected:
    //cache aligned to avoid false-sharing
    struct alignas(std::hardware_destructive_interference_size) Padded19937 {
        std::mt19937 gen;
        Padded19937(unsigned int seed): gen(seed){}
    };

    int population_size;
    int current_population;
    int num_survivor;
    int num_steps;

    std::vector<Individual> population;
    std::vector<double> best_scores; 
    std::vector<double> avg_scores;

    int thread_num;
    std::vector<Padded19937> genrators;
    
    chrono_clock::time_point start_time;
    std::chrono::duration<double> total_time;
    
    // select 2 parents from suriviors using Roulette Wheel approach
    std::vector<int> select_parents(double sum, std::mt19937 &generator);

private:
    // Evaluate fitness score for all current population;
    virtual void evaluate_fitness(int iteration, int print_interval);
    
    // Retain only the top n% of the population, various implementation depending on the GA model.
    virtual void selection_step() = 0;
    
    // performing crossover and mutation to fill the population gap result from selection.
    virtual void update_population();
};