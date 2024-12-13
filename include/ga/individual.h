#pragma once

#include "game/util.h"
#include "nn/neural_network.h"

#include <random>

class Individual {
public:
    Individual();

    // initialize with predefined network
    Individual(MLP&& network);

    Individual(const Individual& other) = default;
    ~Individual() = default;

    Individual& operator=(Individual&& other) noexcept = default;

    Direction get_direction(std::vector<float>& features);
    void save(const std::string& filename);

    void mutate(std::mt19937& rng);
    std::vector<Individual> crossover(std::mt19937& rng, const Individual& other);
    double calculate_fitness(std::mt19937& rng);
    double get_fitness(){ return _fitness; }
private:
    MLP network;
    double _fitness = 0;
};