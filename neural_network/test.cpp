#include "neural_network.h"

#include <iostream>

int main() {
    int layer_sizes[] = {2, 3, 1};
    MLP mlp(layer_sizes, 2, "relu", "sigmoid");

    float input[] = {1, 2};
    float output[1];

    mlp.forward(input, output);
    std::cout << output[0] << std::endl;

    mlp.save("model.bin");

    MLP mlp2("model.bin");

    mlp2.forward(input, output);

    std::cout << output[0] << std::endl;
    return 0;
}