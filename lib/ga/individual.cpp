#include "ga/individual.h"

Individual::Individual() {
    int layer_sizes[4] = {32, 20, 12, 4};
    network = MLP(layer_sizes, 4, "relu", "sigmoid");
}

Direction Individual::get_direction(std::vector<float>& features) {
    network.forward(features);
    // argmax for features
    int max_idx = 0;
    for (int i = 1; i < features.size(); i++) {
        if (features[i] > features[max_idx]) max_idx = i;
    }
    return static_cast<Direction>(max_idx);
}