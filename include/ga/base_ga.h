#pragma once

#include "ga/individual.h"
#include <random>
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

    BaseGA(int populationSize, int numSteps);
    ~BaseGA() = default;

private:
    std::vector<Individual> population;
    std::vector<double> bestScores; 
    std::vector<double> avgScores;
    
    int populationSize;
    int currentPopulation = 0;
    int numSurvivor;
    int numSteps;

    std::mt19937 gen;
    
    // Evaluate fitness score for all current population;
    void EvaluateFitness(int iteration);
    
    // Retain only the top n% of the population, various implementation depending on the GA model.
    virtual void SelectionStep() = 0;
    
    // performing crossover and mutation to fill the population gap result from selection.
    void updatePopulation();

    // select 2 parents from suriviors using Roulette Wheel approach
    std::vector<int> selectParents(double sum);
};