//
// Created by ohbra on 4/25/2020.
//

#include "Location.h"
Location::Location(float input_x, float input_y) {
  x = input_x;
  y = input_y;
}
float Location::GetX() const { return x; }
float Location::GetY() const { return y; }

bool Location::operator==(const Location& rhs) const { return rhs.x == this->x && rhs.x == this->y; }
float Location::distance(Location location) {
  float x_squared = pow((location.GetX() - x), 2);
  float y_squared = pow((location.GetY() - y), 2);

  return sqrt(x_squared + y_squared);
}
