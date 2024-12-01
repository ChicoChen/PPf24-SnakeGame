#include "game/game.h"
#include <iostream>
int main() {
  Game game;
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
  }
}
