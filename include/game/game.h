#include "game/snake.h"
#include "game/util.h"
#include <vector>

class Game {
public:
  Game();
  bool run(Ctrl ctrl);
  std::vector<float> get_features();

private:
};
