#include "ga/base_ga.h"
#include <omp.h>
#include <algorithm>

Individual BaseGA::GetBestIndividual(){
    double maxScore = 0;
    int bestIndvIdx = -1;
    
    #pragma omp parallel
    {
        int localMax = 0;
        int localidx = -1;
        #pragma omp for
        for(int i = 0; i < populationSize; i++){
            double score = population[i].fitness();
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
    //todo: track avg Score and best Score of each step.
    for(int i = 0; i < numSteps; i++){
        EvaluateFitness();
        SelectionStep();
        updatePopulation();
    }
}

BaseGA::BaseGA(int populationSize, int numSteps):
    population(populationSize),
    bestScores(numSteps, 0),
    avgScores(numSteps, 0),
    populationSize(populationSize),
    numSteps(numSteps),
    numSurvivor(0), //todo: build an env file to handle hyperparameters 
    gen(std::random_device{}()){}

void EvaluateFitness(){
    //todo: evaluate fitness of populations
    //* potential chance for parallel.
}


void BaseGA::updatePopulation(){
    double sum = 0;
    for(int i = 0; i < numSurvivor; i++){
        sum += population[i].fitness();
    }
    
    // shuffle survivors to randomize parent slection
    std::shuffle(population.begin(), population.begin() + numSurvivor, gen);
    
    while(currentPopulation < populationSize){
        std::vector<int> par = selectParents(sum);
        Individual &father = population[par[0]];
        Individual &mother = population[par[1]];
        std::vector<Individual> children = father.crossover(mother);
        for(auto &child : children){
            child.mutate(); // currently do nothing
            population[currentPopulation] = child; //? how about using std::move()?
            currentPopulation++;
        }
    }
}

std::vector<int> BaseGA::selectParents(double sum){
    static int idx;
    std::vector<int> parents;
    std::uniform_real_distribution<> dis(0.0 , sum);

    for(int i = 0; i < 2; i++){
        double threshold = dis(gen);
        double count = 0;
        for(int candidate = 0; candidate < numSurvivor; candidate++){
            count += population[candidate].fitness();
            if(count >= threshold){
                parents.push_back(candidate);
                break;
            }
        }
    }

    return parents;    
}