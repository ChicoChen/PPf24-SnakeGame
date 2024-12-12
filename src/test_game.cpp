#include "game/game.h"
#include <iostream>
#include <random>
int main() {
  std::random_device rd;
  std::mt19937 rng(rd());
  Game game(rng);
  char ctrl;
  game.dump();
  while (std::cin >> ctrl) {
    Direction direction;
    switch (ctrl) {
      case 'w':
        direction = Direction::Top;
        break;
      case 's':
        direction = Direction::Bottom;
        break;
      case 'a':
        direction = Direction::Left;
        break;
      case 'd':
        direction = Direction::Right;
        break;
      default:
        continue;
    }
    if (!game.run(direction)) {
      break;
    }
    game.dump();
    std::vector<float> features = game.get_features();
    for (auto &f : features) {
      printf("%f ", f);
    }
    printf("\n");
  }
}
