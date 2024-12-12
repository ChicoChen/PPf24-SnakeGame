#include "game/game.h"
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <random>


static std::pair<int, int> eight_directions[8] = {
  {-1, -1}, {-1, 0}, {-1, 1},
  {0, -1}, {0, 1},
  {1, -1}, {1, 0}, {1, 1}
};

Game::Game(std::mt19937 &rng) : rng(rng) {
  features.clear();
  features.resize(32);
  snake = Snake();
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      food_cand.insert(Point(i, j));
    }
  }
  food_cand.erase(snake.get_body().front());
  int idx_rand = rng() % food_cand.size();
  auto it = food_cand.begin();
  std::advance(it, idx_rand);
  food = *it;
  food_cand.erase(it);

  // random move 2 times
  int dir = rng() % 4 + 1;
  snake.move(static_cast<Direction>(dir));
  snake.grow();
  dir = rng() % 4 + 1;
  snake.move(static_cast<Direction>(dir));
  snake.grow();
}

bool Game::run(Direction ctrl) {
  if (snake.move(ctrl)) {
    return false;
  }
  if (snake.get_body().front().x < 0 || snake.get_body().front().x >= 10 ||
      snake.get_body().front().y < 0 || snake.get_body().front().y >= 10) {
    return false;
  }
  food_cand.erase(snake.get_body().front());
  frame++;
  if (snake.get_body().front().x == food.x && snake.get_body().front().y == food.y) {
    snake.grow();
    int idx_rand = rng() % food_cand.size();
    auto it = food_cand.begin();
    std::advance(it, idx_rand);
    food = *it;
    food_cand.erase(it);
    score++;
    starvation = 10 * 10;
  }
  else {
    food_cand.insert(snake.get_last_pos());
    starvation--;
    if (starvation == 0) {
      return false;
    }
  }
  return true;
}

void Game::dump() const {
  // print flush
  printf("\033[2J\033[1;1H");
  // printf("%d\n", flag);
   
  std::vector<std::vector<int> > board(10, std::vector<int>(10, 0));
  for (auto &p : snake.get_body()) {
    board[p.y][p.x] = 1;
  }
  board[food.y][food.x] = 2;
  printf("############\n");
  for (int i = 0; i < 10; i++) {
    printf("#");
    for (int j = 0; j < 10; j++) {
      if (board[i][j] == 0) {
        printf(" ");
      } else if (board[i][j] == 1) {
        printf("O");
      } else {
        printf("*");
      }
    }
    printf("#\n");
  }
  printf("############\n");
}

std::vector<float> &Game::get_features() {
  // std::vector<float> features;
  // features.reserve(32);
  std::vector<std::vector<int> > board(10, std::vector<int>(10, 0));
  for (auto &p : snake.get_body()) {
    board[p.y][p.x] = 1;
  }
  board[food.y][food.x] = 2;
  Point head = snake.get_body().front();
  for (int i = 0; i < 8; i++) {
    float dist_to_apple = 0.0;
    float dist_to_self = 0.0;
    float dist_to_wall = 0.0;
    float dist = 1.0;
    Point now = head;
    now.x += eight_directions[i].second;
    now.y += eight_directions[i].first;
    bool self_found = false;
    bool food_found = false;
    while (now.x >= 0 && now.x < 10 && now.y >= 0 && now.y < 10) {
      if (!self_found && board[now.y][now.x] == 1) {
        dist_to_self = 1.0;
        self_found = true;
      }
      if (!food_found && board[now.y][now.x] == 2) {
        dist_to_apple = 1.0;
        food_found = true;
      }
      now.x += eight_directions[i].second;
      now.y += eight_directions[i].first;
      dist += sqrt(eight_directions[i].second * eight_directions[i].second + eight_directions[i].first * eight_directions[i].first);
    }
    dist_to_wall = 1 / dist;
    features[i * 3] = (dist_to_apple);
    features[i * 3 + 1] =(dist_to_self);
    features[i * 3 + 2] = (dist_to_wall);
  }
  for (int i = 1; i <= 4; i++) {
    features[23 + i] = snake.head_direction() == i;
  }
  for (int i = 1; i <= 4; i++) {
    features[27 + i] = snake.tail_direction() == i;
  }
  return features; 
}

float Game::calculate_fitness() const {
  // self._fitness = self._frames + ((2 ** self.score) + (self.score ** 2.1) * 100) - ((.25 * self._frames) ** 1.3) * (self.score ** 1.2))
  return frame + (pow(2, score) + pow(score, 2.1) * 100) - (pow(0.25 * frame, 1.3) * pow(score, 1.2));
}
