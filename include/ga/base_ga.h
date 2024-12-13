#pragma once

#include "ga/individual.h"
#include <random>
#include <vector>

#define DEFAULT_POPULATION 100
#define DEFAULT_STEP 5
#define SURVIVAL_RATE 0.5

/*
Base class for all genetic algorithm implementations.
This class serves as the foundation for implementing various genetic algorithms. 
It provides common methods such as ...
*/
class BaseGA{
public:
    // return the best individual, parallelized by omp
    virtual const Individual& GetBestIndividual();
    // perform the whole selection process.
    void performSelection();

    BaseGA(int populationSize = DEFAULT_POPULATION, int numSteps = DEFAULT_STEP, int thread_num = 1);
    ~BaseGA() = default;

protected:
    std::vector<Individual>& getPopulation(){ return population; }
    int populationSize;
    int currentPopulation;
    int numSurvivor;
    int numSteps;

private:
    std::vector<Individual> population;
    std::vector<double> bestScores; 
    std::vector<double> avgScores;

    int thread_num;
    std::vector<std::mt19937> genrators;
    
    // Evaluate fitness score for all current population;
    virtual void EvaluateFitness(int iteration);
    
    // Retain only the top n% of the population, various implementation depending on the GA model.
    virtual void SelectionStep() = 0;
    
    // performing crossover and mutation to fill the population gap result from selection.
    virtual void updatePopulation();

    // select 2 parents from suriviors using Roulette Wheel approach
    std::vector<int> selectParents(double sum);
};