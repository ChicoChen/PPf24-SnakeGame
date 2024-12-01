#pragma once
#include <utility>
enum Direction {Top, Bottom, Left, Right};

struct Point : public std::pair<int, int> {
  Point() : Point(0, 0) {}
  int x, y;
  Point(int x, int y) : x(x), y(y) {}
};
