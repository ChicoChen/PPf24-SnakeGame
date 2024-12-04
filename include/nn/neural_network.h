#pragma once

#include "activation_func.h"

#include <vector>
#include <memory>

class Layer {
public:
    Layer(int input_size, int output_size, const std::string& activation_name);
    Layer(std::ifstream& file);
    ~Layer() = default;

    Layer(Layer&& other) noexcept;
    Layer& operator=(Layer&& other) noexcept;

    Layer(const Layer&) = delete;
    Layer& operator=(const Layer&) = delete;

    std::vector<float> forward(std::vector<float>& input);
    void set_weights(float* new_weights);
    void save(std::ofstream& file);

private:
    int input_size;
    int output_size;
    Activation activation;
    float(*activation_func)(float);
    std::unique_ptr<float[]> weights; // (input_size + 1) * output_size, bias in the last column
};

class MLP {
public:
    MLP();
    MLP(int* layer_sizes, int num_layers, const std::string& layer_activation, const std::string& output_activation);
    MLP(const std::string& filename);  // Load weights from file

    std::vector<float> forward(std::vector<float>& input);
    void save(const std::string& filename);

private:
    std::vector<Layer> layers;
};