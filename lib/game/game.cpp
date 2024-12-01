#include "game/game.h"


int flag = 0;
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
    flag = 1;
    snake.grow();
    food = Point(rand() % 10, rand() % 10);
  }
  return true;
}

void Game::dump() const {
  // print flush
  printf("\033[2J\033[1;1H");
  printf("%d\n", flag);
  flag = 0;
   
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