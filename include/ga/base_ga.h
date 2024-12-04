#pragma once

#include "ga/individual.h"
#include <vector>

/*
Base class for all genetic algorithm implementations.
This class serves as the foundation for implementing various genetic algorithms. 
It provides common methods such as ...
*/
class BaseGA{
public:
    // return the best individual, parallelized by omp
    Individual GetBestIndividual();
    // perform the whole selection process.
    void performSelection();

    //todo constructor & destructor
    BaseGA(int populationSize, int numSteps);
    ~BaseGA() = default;

private:
    std::vector<Individual> population;
    std::vector<double> fitness;
    std::vector<double> bestScores;
    std::vector<double> avgScores;
    
    int populationSize;
    int numSteps;
    
    // Evaluate fitness score for all current population;
    void EvaluateFitness();
    // Retain only the top n% of the population, various implementation depending on the GA model.
    virtual void SelectionStep() = 0;
    // fill the population gap using crossover or mutation
    void updatePopulation();
    void PerformCrossover();
    void PerformMutation();
};