#include "game/snake.h"
#include "game/util.h"
#include <random>
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
  int frame = 0;
  int score = 0;
  int starvation = 100;
  std::random_device rd;
  std::mt19937 gen{rd()};
  // std::uniform_int_distribution<> dis(1, 10000);
};
