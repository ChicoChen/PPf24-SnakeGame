#pragma once

#include <random>

#include "game/util.h"
#include "nn/neural_network.h"

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

    void mutate(int idx);
    std::vector<Individual> crossover(const Individual& other);

    // each call to `evaluate()`, the fitness score will be re-calculated
    void evaluate(std::mt19937& rng);

    // score and fitness calculated by `evaluate()`, cached
    int score;
    double fitness;

   private:
    MLP network;
};