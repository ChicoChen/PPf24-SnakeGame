#include "ga/individual.h"
#include "game/game.h"

#include <algorithm>
#include <random>
#include <iostream>

#define stddev 0.3

constexpr int layer_sizes[4] = {32, 20, 12, 4};

Individual::Individual()
    : network(layer_sizes, 4, "relu", "sigmoid"){}

Direction Individual::get_direction(std::vector<float>& features) {
    std::vector<float> output = network.forward(features);
    // argmax for features
    int max_idx = 0;
    for (int i = 1; i < output.size(); i++) {
        if (output[i] > output[max_idx]) max_idx = i;
    }
    return static_cast<Direction>(max_idx);
}

void Individual::mutate(){
    std::vector<Layer>& network = this->getLayers();
    int layerNum = network.size();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(0.f, stddev);
    
    for(int i = 0 ; i < layerNum; i++){
        size_t weightSize = network[i].getWeightSize();
        float *new_weight = new float[weightSize];
        std::copy(network[i].getWeight(), network[i].getWeight() + weightSize, new_weight);

        for(int w = 0; w < weightSize; w++){
            float sum = new_weight[w] + dist(gen);
            new_weight[w] = std::min(1.f, std::max(-1.f, sum));
        }
        network[i].set_weights(new_weight);
        delete[] new_weight;
    }
}

std::vector<Individual> Individual::crossover(const Individual& other){
    Individual child1(*this);
    Individual child2(other);

    std::vector<Layer>& network1 = child1.getLayers();
    std::vector<Layer>& network2 = child2.getLayers();
    int layerNum = network1.size();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);
    for(int i = 0 ; i < layerNum; i++){
        size_t weightSize = network1[i].getWeightSize();
        float *new_weight1 = new float[weightSize];
        float *new_weight2 = new float[weightSize];

        std::copy(network1[i].getWeight(), network1[i].getWeight() + weightSize, new_weight1);
        std::copy(network2[i].getWeight(), network2[i].getWeight() + weightSize, new_weight2);
        
        for(int w = 0; w < weightSize; w++){
            if(dis(gen) >5) std::swap(new_weight1[w], new_weight2[w]);
        }

        network1[i].set_weights(new_weight1);
        network2[i].set_weights(new_weight2);
        delete[] new_weight1;
        delete[] new_weight2;
    }

    std::vector<Individual> offspring;
    offspring.push_back(child1);
    offspring.push_back(child2);
    return offspring;
}

double Individual::fitness() {
    if (_fitness != 0) {
        // fitness already calculated, return cached value
        return _fitness;
    }
    // calculate fitness
    Game game = Game();
    Direction direction;

    do {
        direction = get_direction(game.get_features());

        #ifdef SHOWGAMEBOARD
        game.dump();
        std::cout << (int)direction << '\n';
        #endif
    } while (game.run(direction));

    _fitness = game.calculate_fitness();
    return _fitness;
}

std::vector<Layer>& Individual::getLayers(){
    return network.getLayers();
}