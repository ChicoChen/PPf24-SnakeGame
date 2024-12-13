#include "ga/base_ga.h"

#include <iostream>
#include <omp.h>
#include <cassert>
#include <algorithm>

const Individual& BaseGA::GetBestIndividual(){
    int bestIndvIdx = -1;
    double maxScore = 0;
    for(int i = 0; i < populationSize; i++){
        double score = population[i].get_fitness();
        if(score > maxScore){
            maxScore = score;
            bestIndvIdx = i;
        }
    }
    
    assert(bestIndvIdx >= 0);
    return population[bestIndvIdx];
}

void BaseGA::performSelection(){
    for(int i = 0; i < numSteps; i++){
        #ifdef DEBUG
        std::cout << "--------------------------------" << std::endl;
        std::cout << "[baseGA] begin selection round: " << i << std::endl;
        #endif
        EvaluateFitness(i);
        SelectionStep();
        updatePopulation();
    }
}

//TODO: build an env file to handle hyperparameters 
BaseGA::BaseGA(int populationSize, int numSteps, int thread_num):
    population(populationSize),
    populationSize(populationSize),
    currentPopulation(populationSize),
    bestScores(numSteps, 0),
    avgScores(numSteps, 0),
    numSteps(numSteps),
    numSurvivor(populationSize * SURVIVAL_RATE),
    thread_num(thread_num){
        // construct one generator for each thread. default is serial
        std::random_device rd;
        for(int i = 0; i < thread_num; i++){
            genrators.emplace_back(rd());
        }
    }

//when Individual.fitness() is called, fitness score of each object is calculated and cached for furture access
void BaseGA::EvaluateFitness(int iteration){
    //* potential chance for parallel.
    for(int i = 0; i < populationSize; i++){
        double fitness = population[i].calculate_fitness(genrators[0]);

        #ifdef DEBUG
        std::cout << "[baseGA] evaluating agent: " << i
                  << "\tscore: " << fitness << std::endl;
        #endif
        avgScores[iteration] += fitness;
        bestScores[iteration] = (fitness > bestScores[iteration])?
                                fitness:
                                bestScores[iteration];
    }
    avgScores[iteration] /= populationSize;

    #ifdef DEBUG
    //TODO: another cmake macro for fine-grained output messages like this
    // std::cout << "[baseGA] bestScores: " << bestScores[iteration] <<
    //              ", avgScores: " << avgScores[iteration] << std::endl;
    #endif
}


void BaseGA::updatePopulation(){
    double sum = 0;
    for(int i = 0; i < numSurvivor; i++){
        sum += population[i].get_fitness();
    }
    
    // shuffle survivors to randomize parent slection
    std::shuffle(population.begin(), population.begin() + numSurvivor, genrators[0]);
    while(currentPopulation < populationSize){
        std::vector<int> par = selectParents(sum);
        Individual &father = population[par[0]];
        Individual &mother = population[par[1]];

        for(auto &child : father.crossover(genrators[0], mother)){
            child.mutate(genrators[0]);
            population[currentPopulation++] = std::move(child);
            //edge condition: only need to add one child 
            if(currentPopulation == populationSize) break;
        }
    }

    #ifdef DEBUG
    std::cout << "[baseGA] population updated" << std::endl;
    #endif
}

std::vector<int> BaseGA::selectParents(double sum){
    assert(numSurvivor >= 2 && sum >= 0);

    static int idx;
    std::vector<int> parents;
    std::uniform_real_distribution<> dis(0.0 , sum);

    for(int i = 0; i < 2; i++){
        double threshold = dis(genrators[0]);
        double count = 0;
        for(int candidate = 0; candidate < numSurvivor; candidate++){
            count += population[candidate].get_fitness();
            if(count >= threshold){
                parents.push_back(candidate);
                break;
            }
        }
    }

    return parents;    
}