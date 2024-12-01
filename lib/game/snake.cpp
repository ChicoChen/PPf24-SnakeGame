#include "game/snake.h"

Snake::Snake() {
  body.push_back(Point(5, 5));
}

bool Snake::move(Direction ctrl) {
  Point head = body[0];
  Point new_head = head;
  switch (ctrl) {
    case Direction::Top:
      new_head.y -= 1;
      break;
    case Direction::Bottom:
      new_head.y += 1;
      break;
    case Direction::Left:
      new_head.x -= 1;
      break;
    case Direction::Right:
      new_head.x += 1;
      break;
  }
  last_pos = body.back();
  body.pop_back();
  for (auto &p : body) {
    if (p.x == new_head.x && p.y == new_head.y) {
      return true;
    }
  }
  body.push_front(new_head);
  return false;
}

void Snake::grow() {
  printf("grow\n");
  body.push_back(last_pos);
}

const std::deque<Point> &Snake::get_body() const {
  return body;
}