
#include <vector>

// this is a temporary dummy class, stands for a NN player(a combination of weight)
class Agent{};

/*
Base class for all genetic algorithm implementations.
This class serves as the foundation for implementing various genetic algorithms. 
It provides common methods such as ...
*/
class BaseGA{
public:
    void InitializePopulation();
    Agent GetBestIndividual();
    void performSecletion();

    //todo constructor & destructor
    BaseGA(int populationSize, int numSteps);
    ~BaseGA() = default;

private:
    std::vector<Agent> population;
    std::vector<double> bestScores;
    std::vector<double> avgScores;
    
    int populationSize;
    int numSteps;

    virtual void SelectionStep() = 0;
    void EvaluateFitness();
    double GetFitnessScore(const Agent &agent);
    void updatePopulation();
    Agent PerformCrossover(const Agent &parent1, const Agent &parent2);
    void PerformMutation(Agent &mutant);
};