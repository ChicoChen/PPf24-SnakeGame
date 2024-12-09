#pragma once

#include "game/util.h"
#include "nn/neural_network.h"

class Individual {
public:
    Individual();
    Individual(const Individual& other) = default;
    ~Individual() = default;

    Individual& operator=(Individual&& other) noexcept = default;

    Direction get_direction(std::vector<float>& features);
    void save(const std::string& filename);

    void mutate();
    std::vector<Individual> crossover(const Individual& other);
    double fitness();

private:
    MLP network;
    double _fitness = 0;
    
    std::vector<Layer>& getLayers();
};