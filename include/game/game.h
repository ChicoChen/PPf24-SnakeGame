#include "game/snake.h"
#include "game/util.h"
#include <set>
#include <vector>

class Game {
public:
  Game();
  bool run(Direction ctrl);
  std::vector<float> &get_features() ;
  void dump() const;
  float calculate_fitness() const;

private:
  Snake snake;
  Point food;
  std::set<Point> food_cand;
  std::vector<float> features;
  int frame;
  int score;
};
