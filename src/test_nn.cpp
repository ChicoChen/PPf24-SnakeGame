#include <iostream>

#include "nn/neural_network.h"

int main() {
    int layer_sizes[] = {2, 3, 1};
    MLP mlp(layer_sizes, 2, "relu", "sigmoid");

    std::vector<float> arr{1, 2};

    std::vector<float> output = mlp.forward(arr);
    std::cout << output[0] << std::endl;

    for (int i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    mlp.save("model.bin");

    MLP mlp2("model.bin");
    output = mlp2.forward(arr);
    std::cout << output[0] << std::endl;
    return 0;
}