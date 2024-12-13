#include "ga/individual.h"
#include "game/game.h"
#include "nn/neural_network.h"

#include <algorithm>
#include <random>
#include <iostream>

#define MUTATION_RATE 0.05

constexpr int layer_sizes[4] = {32, 20, 12, 4};

std::uniform_real_distribution<float> uni_dist(0.f, 1.f);
std::normal_distribution<float> norm_dist(0.f, 1.f);

Individual::Individual()
    : network(layer_sizes, 3, "relu", "sigmoid"){}

Individual::Individual(MLP&& network) : network(std::move(network)) {}

Direction Individual::get_direction(std::vector<float>& features) {
    std::vector<float> output = network.forward(features);
    // argmax for features
    int max_idx = 0;
    for (int i = 1; i < output.size(); i++) {
        if (output[i] > output[max_idx]) max_idx = i;
    }
    return static_cast<Direction>(max_idx + 1);
}

void Individual::save(const std::string& filename){
    #ifdef DEBUG
    std::cout << "model exported as " << filename << std::endl;
    #endif

    network.save(filename);
}

void Individual::mutate(std::mt19937& rng){
    std::vector<Layer>& layers = network.layers;

    size_t n_layers = layers.size();
    
    std::normal_distribution<float> dist(0.f, 1.f);

    for(int i = 0 ; i < n_layers; i++){
        size_t weights_size = layers[i].get_weights_size();
        float* weights = layers[i].get_weights();

        for(int j = 0; j < weights_size; j++){
            if (uni_dist(rng) < MUTATION_RATE) weights[j] += norm_dist(rng);
        }
    }
}

std::vector<Individual> Individual::crossover(std::mt19937& rng, const Individual& other){
    const std::vector<Layer>& parent1 = network.layers;
    const std::vector<Layer>& parent2 = other.network.layers;

    size_t n_layers = parent1.size();
    
    // copy parent layers
    std::vector<Layer> child1(parent1), child2(parent2);

    for (int i = 0 ; i < n_layers; i++){
        size_t weights_size = child1[i].get_weights_size();

        float* w1 = child1[i].get_weights();
        float* w2 = child2[i].get_weights();

        for (int j = 0; j < weights_size; j++){
            if (uni_dist(rng) < 0.5){
                std::swap(w1[j], w2[j]);
            }
        }
    }

    std::vector<Individual> offsprings;
    offsprings.emplace_back(MLP(child1));
    offsprings.emplace_back(MLP(child2));
    return offsprings;
}

double Individual::fitness(std::mt19937 &rng) {
    // calculate fitness
    Game game = Game(rng);
    Direction direction;

    do {
        direction = get_direction(game.get_features());

        #ifdef SHOWGAMEBOARD
        game.dump();
        std::cout << (int)direction << '\n';
        #endif
    } while (game.run(direction));

    return game.calculate_fitness();
}