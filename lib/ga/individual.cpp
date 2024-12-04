#include "ga/individual.h"
#include "game/game.h"

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

double Individual::fitness() {
    if (_fitness != 0) {
        // fitness already calculated, return cached value
        return _fitness;
    }
    // calculate fitness
    Game game = Game();
    Direction direction;

    do {
        std::vector<float> features = game.get_features();
        direction = get_direction(features);
    } while (game.run(direction));

    _fitness = game.calculate_fitness();
    return _fitness;
}