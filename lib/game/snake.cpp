#include "game/snake.h"
#include "game/util.h"

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

Direction Snake::head_direction() const {
  if (body.size() < 2) {
    return Direction::None;
  }
  Point head = body[0];
  Point neck = body[1];
  if (head.x == neck.x) {
    if (head.y > neck.y) {
      return Direction::Top;
    } else {
      return Direction::Bottom;
    }
  } else {
    if (head.x > neck.x) {
      return Direction::Left;
    } else {
      return Direction::Right;
    }
  }
}

Direction Snake::tail_direction() const {
  if (body.size() < 2) {
    return Direction::None;
  }
  Point tail = body.back();
  Point last = last_pos;
  if (tail.x == last.x) {
    if (tail.y > last.y) {
      return Direction::Top;
    } else {
      return Direction::Bottom;
    }
  } else {
    if (tail.x > last.x) {
      return Direction::Left;
    } else {
      return Direction::Right;
    }
  }
}