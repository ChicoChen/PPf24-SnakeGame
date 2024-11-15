
#include "snake.h"

#include <vector>

class BaseGA{
/**
 * @brief Base class for all genetic algorithm implementations.
 * 
 * This class serves as the foundation for implementing various genetic algorithms. 
 * It provides common methods such as ...
 */
public:
    void InitializePopulation();
    snake GetBestIndividual();
    void performSecletion();

    //todo constructor & destructor

private:
    std::vector<snake> population;
    std::vector<double> bestScores;
    std::vector<double> avgScores;
    
    int populationSize;
    int numSteps;

    virtual void SelectionStep() = 0;
    void EvaluateFitness();
    double GetFitnessScore(const snake &agent);
    void updatePopulation();
    snake PerformCrossover(const snake &parent1, const snake &parent2);
    void PerformMutation(snake &mutant);
};