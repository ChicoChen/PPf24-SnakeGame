#include "game/game.h"
#include <cmath>


std::pair<int, int> eight_directions[8] = {
  {-1, -1}, {-1, 0}, {-1, 1},
  {0, -1}, {0, 1},
  {1, -1}, {1, 0}, {1, 1}
};

Game::Game() {
  snake = Snake();
  // food.insert(Point(5, 5));
  food = Point(rand() % 10, rand() % 10);
}

bool Game::run(Direction ctrl) {
  if (snake.move(ctrl)) {
    return false;
  }
  if (snake.get_body().front().x < 0 || snake.get_body().front().x >= 10 ||
      snake.get_body().front().y < 0 || snake.get_body().front().y >= 10) {
    return false;
  }
  if (snake.get_body().front().x == food.x && snake.get_body().front().y == food.y) {
    snake.grow();
    food = Point(rand() % 10, rand() % 10);
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

std::vector<float> Game::get_features() const {
  std::vector<float> features;
  features.reserve(32);
  std::vector<std::vector<int> > board(10, std::vector<int>(10, 0));
  for (auto &p : snake.get_body()) {
    board[p.y][p.x] = 1;
  }
  board[food.y][food.x] = 2;
  Point head = snake.get_body().front();
  // Point tail = snake.get_body().back();
  for (int i = 0; i < 8; i++) {
    float dist_to_apple = 0.0;
    float dist_to_self = 0.0;
    float dist_to_wall = 0.0;
    float dist = 1.0;
    Point now = head;
    bool slef_found = false;
    bool food_found = false;
    while (now.x >= 0 && now.x < 10 && now.y >= 0 && now.y < 10) {
      if (!slef_found && board[now.y][now.x] == 1) {
        dist_to_self = 1.0;
        slef_found = true;
      }
      if (!food_found && board[now.y][now.x] == 2) {
        dist_to_apple = 1.0;
        food_found = true;
      }
      now.x += eight_directions[i].second;
      now.y += eight_directions[i].first;
      dist += 1.0;
    }
    dist_to_wall = 1 / dist;
    features.push_back(dist_to_apple);
    features.push_back(dist_to_self);
    features.push_back(dist_to_wall);
  }
  for (int i = 0; i < 4; i++) {
    
  }
  return features; 
}