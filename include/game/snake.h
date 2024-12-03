#include "game/util.h"
#include <deque>
class Snake {
public:
  // snake default
  Snake();
  bool move(Direction ctrl);
  void grow();
  const std::deque<Point> &get_body() const;
  Direction head_direction() const;
  Direction tail_direction() const; 
  const Point &get_last_pos() const;
private:
  std::deque<Point> body;
  Point last_pos;
};
