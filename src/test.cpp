#include "nn/neural_network.h"

#include <iostream>

int main() {
    int layer_sizes[] = {2, 3, 1};
    MLP mlp(layer_sizes, 2, "relu", "sigmoid");

    std::vector<float> arr{1, 2};

    mlp.forward(arr);
    std::cout << arr[0] << std::endl;

    mlp.save("model.bin");

    MLP mlp2("model.bin");

    mlp2.forward(arr);

    std::cout << arr[0] << std::endl;
    return 0;
}