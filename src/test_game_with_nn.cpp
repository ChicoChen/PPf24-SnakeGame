#include "nn/neural_network.h"
#include "game/game.h"
#include <chrono>
#include <iostream>
#include <string>
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
  std::string model_name = "demo_1000_100";
  MLP mlp(model_name);
  // printf("Model loaded\n");
  std::random_device rd;
  std::mt19937 rng(rd());
  Game game = Game(rng);
  Direction dir;
  do {
    game.dump();
    printf("Direction: %d\n", dir);
    dir = get_direction(game, mlp);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  } while (game.run(dir));
  std::cout << game.calculate_fitness() << std::endl;
}