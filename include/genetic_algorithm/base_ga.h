
#inlcude "nn/individual.h"

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
    void performSecletion();

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

    void EvaluateFitness();
    virtual void SelectionStep() = 0;
    void updatePopulation();
    void PerformCrossover();
    void PerformMutation();
};