#include "genetic_algorithm/base_ga.h"
#include <omp.h>

Individual BaseGA::GetBestIndividual(){
    double maxScore = 0;
    int bestIndvIdx = -1;
    
    #pragma omp parallel
    {
        int localMax = 0;
        int localidx = -1;
        #pragma omp for
        for(int i = 0; i < populationSize; i++){
            double score = fitness[i];
            if(score > localMax){
                localMax = score;
                localidx = i;
            }
        }

        #pragma omp critical
        {
            if(localMax > maxScore){
                maxScore = localMax;
                bestIndvIdx = localidx;
            }
        }
    }

    return population[bestIndvIdx];
}

void BaseGA::performSelection(){
    for(int i = 0; i < numSteps; i++){
        EvaluateFitness();
        SelectionStep();
        updatePopulation();
    }
}

BaseGA::BaseGA(int populationSize, int numSteps):
    population(populationSize),
    fitness(numSteps, 0),
    bestScores(numSteps, 0),
    avgScores(numSteps, 0),
    populationSize(populationSize),
    numSteps(numSteps) {}

void BaseGA::EvaluateFitness(){
    #pragma omp for
    for(int i = 0; i < populationSize; i++){
        //TODO: resolve false sharing
        fitness[i] = population[i].fitness();
    }
}

void BaseGA::updatePopulation(){

}