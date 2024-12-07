#include "ga/base_ga.h"
#include <omp.h>
#include <cassert>
#include <algorithm>

Individual BaseGA::GetBestIndividual(){
    int bestIndvIdx = -1;
    double maxScore = 0;
    for(int i = 0; i < populationSize; i++){
        if(population[i].fitness() > maxScore){
            maxScore = population[i].fitness();
            bestIndvIdx = i;
        }
    }
    
    assert(bestIndvIdx >= 0);
    return population[bestIndvIdx];
}

void BaseGA::performSelection(){
    for(int i = 0; i < numSteps; i++){
        EvaluateFitness(i);
        SelectionStep();
        updatePopulation();
    }
}

BaseGA::BaseGA(int populationSize, int numSteps):
    population(populationSize),
    bestScores(numSteps, 0),
    avgScores(numSteps, 0),
    populationSize(populationSize),
    currentPopulation(populationSize),
    numSteps(numSteps),
    numSurvivor(populationSize / 2), //todo: build an env file to handle hyperparameters 
    gen(std::random_device{}()){}

void BaseGA::EvaluateFitness(int iteration){
    //when Individual.fitness() is called, fitness score of each object is calculated and cached for furture access
    //* potential chance for parallel.
    for(int i = 0; i < populationSize; i++){
        avgScores[iteration] += population[i].fitness();
        bestScores[iteration] = (population[i].fitness() > bestScores[iteration])?
                                population[i].fitness():
                                bestScores[iteration];
    }
    avgScores[iteration] /= populationSize ;
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