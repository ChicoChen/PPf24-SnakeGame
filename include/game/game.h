#include "game/snake.h"
#include "game/util.h"
#include <set>
#include <vector>

class Game {
public:
  Game();
  bool run(Direction ctrl);
  std::vector<float> get_features();
  void dump() const;

private:
  Snake snake;
  Point food;
};
