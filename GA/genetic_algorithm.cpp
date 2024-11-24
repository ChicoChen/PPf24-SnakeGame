#include "genetic_algorithm.h"

BaseGA::BaseGA(int populationSize, int numSteps){
    population.reserve(populationSize)
    bestScores.reserve(numSteps)
    avgScores.reserve(numSteps)

    this->populationSize = populationSize;
    this->numSteps = numSteps;
}

Snake BaseGA::GetBestIndividual(){
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
