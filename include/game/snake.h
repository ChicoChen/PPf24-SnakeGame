#include "game/util.h"
#include <vector>
class Snake {
public:
  void move(Ctrl ctrl);
  void grow();
  const std::vector<Point> &get_body() const;

private:
  std::vector<Point> body;
  Point last_pos;
};
