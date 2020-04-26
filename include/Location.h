//
// Created by ohbra on 4/25/2020.
//

#ifndef FINALPROJECT_LOCATION_H
#define FINALPROJECT_LOCATION_H

#include <cmath>

class Location {
 public:
  Location(float input_x, float input_y);
  float GetX() const;
  float GetY() const;
  bool operator==(const Location& rhs) const;
  float distance(Location location);


 private:
  float x;
  float y;
};

#endif  // FINALPROJECT_LOCATION_H
