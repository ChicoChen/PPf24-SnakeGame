#include "nn/neural_network.h"

#include <iostream>
#include <fstream>

MLP::MLP(int* layer_sizes, int num_layers, const std::string& layer_activation, const std::string& output_activation) {
    for (int i = 0; i < num_layers; i++) {
        layers.emplace_back(layer_sizes[i], layer_sizes[i + 1], i == num_layers - 1 ? output_activation : layer_activation);
    }

    print_layers();
}

MLP::MLP(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    int num_layers;
    file >> num_layers;
    for (int i = 0; i < num_layers; i++) {
        layers.emplace_back(file);
    }

    print_layers();
}

std::vector<float> MLP::forward(std::vector<float>& input) {
    std::vector<float> output = layers[0].forward(input);
    for (int i = 1; i < layers.size(); i++) {
        output = layers[i].forward(output);
    }
    return output;
}

void MLP::save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    file << layers.size() << std::endl;
    for (auto& layer : layers) {
        layer.save(file);
    }
}

void MLP::print_layers() {
    for (auto& layer : layers) {
        layer.print_layer();
    }
}


Layer::Layer(int input_size, int output_size, const std::string& activation_name) : input_size(input_size), output_size(output_size) {
    activation = from_string(activation_name);
    activation_func = get_func(activation);

    // random initialization
    size_t weight_size = (input_size + 1) * output_size;
    weights = std::make_unique<float[]>(weight_size);

    // random float from -1 to 1
    for (int i = 0; i < weight_size; i++) {
        weights[i] = (rand() % 2001 - 1000) / 1000.0;
    }
}

Layer::Layer(std::ifstream& file) {
    std::string activation_name;
    file >> input_size >> output_size >> activation_name;

    activation = from_string(activation_name);
    activation_func = get_func(activation);

    size_t weight_size = (input_size + 1) * output_size;
    weights = std::make_unique<float[]>(weight_size);

    for (int i = 0; i < weight_size; i++) {
        file >> weights[i];
    }
}

Layer::Layer(Layer&& other) noexcept
    : input_size(other.input_size), output_size(other.output_size),
    activation(other.activation), activation_func(other.activation_func),
    weights(std::move(other.weights)) {
}

Layer& Layer::operator=(Layer&& other) noexcept {
    if (this != &other) {
        input_size = other.input_size;
        output_size = other.output_size;
        activation = other.activation;
        activation_func = other.activation_func;
        weights = std::move(other.weights);
    }
    return *this;
}

void Layer::set_weights(float* new_weights) {
    std::copy(new_weights, new_weights + (input_size + 1) * output_size, weights.get());
}

std::vector<float> Layer::forward(std::vector<float>& input) {
    std::vector<float> output(output_size);
    for (int i = 0; i < output_size; i++) {
        output[i] = weights[input_size * output_size + i]; // bias
        for (int j = 0; j < input_size; j++) {
            output[i] += input[j] * weights[j * output_size + i];
        }
        output[i] = activation_func(output[i]);
    }
    return output;
}

void Layer::save(std::ofstream& file) {
    file << input_size << " " << output_size << " " << to_string(activation) << std::endl;
    for (int i = 0; i < (input_size + 1) * output_size; i++) {
        file << weights[i] << " ";
    }
    file << std::endl;
}

void Layer::print_layer() {
    for (int i = 0; i < input_size + 1; i++) {
        for (int j = 0; j < output_size; j++) {
            std::cout << weights[i * output_size + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << to_string(activation) << std::endl;
}