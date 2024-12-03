#pragma once
#include <utility>
enum Direction {None, Top, Bottom, Left, Right};

struct Point {
  Point() : Point(0, 0) {}
  int x, y;
  Point(int x, int y) : x(x), y(y) {}
};
