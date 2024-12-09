#include "nn/neural_network.h"
#include "game/game.h"
#include <chrono>
#include <thread>

Direction get_direction(Game& game, MLP& mlp) {
  std::vector<float> features = game.get_features();
  std::vector<float> output = mlp.forward(features);
  int max_idx = 0;
  for (int i = 1; i < output.size(); i++) {
    if (output[i] > output[max_idx]) max_idx = i;
  }
  return static_cast<Direction>(max_idx + 1);
}

int main() {
  MLP mlp("model.bin");
  Game game = Game();
  Direction dir;
  do {
    game.dump();
    dir = get_direction(game, mlp);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  } while (game.run(dir));
}