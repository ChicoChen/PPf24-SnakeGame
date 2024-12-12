#include "game/snake.h"
#include "game/util.h"
#include <set>
#include <vector>
#include <random>

class Game {
public:
  Game(std::mt19937 &rng);
  bool run(Direction ctrl);
  std::vector<float> &get_features() ;
  void dump() const;
  float calculate_fitness() const;

private:
  Snake snake;
  Point food;
  std::set<Point> food_cand;
  std::vector<float> features;
  int frame = 0;
  int score = 0;
  int starvation = 10 * 10;
  std::mt19937 &rng;
};
