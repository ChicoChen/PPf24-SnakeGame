#include "genetic_algorithm/base_ga.h"

BaseGA::BaseGA(int populationSize, int numSteps){
    population.reserve(populationSize);
    bestScores.reserve(numSteps);
    avgScores.reserve(numSteps);

    this->populationSize = populationSize;
    this->numSteps = numSteps;
}

Agent BaseGA::GetBestIndividual(){
    double maxScore = 0;
    for(auto snake: population){
        //TODO: calculate each snake's fitness
    }
}

void BaseGA::performSecletion(){
    for(int i = 0; i < numSteps; i++){
        EvaluateFitness();
        SelectionStep();
        updatePopulation();
    }
}
