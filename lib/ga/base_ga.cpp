#include "ga/base_ga.h"

#include <iostream>
#include <omp.h>
#include <cassert>
#include <algorithm>

const Individual& BaseGA::GetBestIndividual(){
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
        #ifdef DEBUG
        std::cout << "[baseGA] begin selection round: " << i << std::endl;
        #endif
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
    numSurvivor(populationSize * SURVIVAL_RATE), //todo: build an env file to handle hyperparameters 
    gen(std::random_device{}()){}

void BaseGA::EvaluateFitness(int iteration){
    //when Individual.fitness() is called, fitness score of each object is calculated and cached for furture access
    //* potential chance for parallel.
    for(int i = 0; i < populationSize; i++){
        #ifdef DEBUG
        std::cout << "[baseGA] evaluating agent: " << i << std::endl;
        #endif

        avgScores[iteration] += population[i].fitness();
        bestScores[iteration] = (population[i].fitness() > bestScores[iteration])?
                                population[i].fitness():
                                bestScores[iteration];
    }
    avgScores[iteration] /= populationSize;
}


void BaseGA::updatePopulation(){
    #ifdef DEBUG
    std::cout << "[baseGA] population updated" << std::endl;
    #endif

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
            child.mutate(); //TODO: don't mutate all the child
            population[currentPopulation] = std::move(child);
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