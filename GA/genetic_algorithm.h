
#include "Snake.h"

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
    Snake GetBestIndividual();
    void performSecletion();

    //todo constructor & destructor
    BaseGA(int populationSize, int numSteps);
    ~BaseGA() = default;

private:
    std::vector<Snake> population;
    std::vector<double> bestScores;
    std::vector<double> avgScores;
    
    int populationSize;
    int numSteps;

    virtual void SelectionStep() = 0;
    void EvaluateFitness();
    double GetFitnessScore(const Snake &agent);
    void updatePopulation();
    Snake PerformCrossover(const Snake &parent1, const Snake &parent2);
    void PerformMutation(Snake &mutant);
};