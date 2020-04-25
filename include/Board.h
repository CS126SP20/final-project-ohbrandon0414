//
// Created by ohbra on 4/20/2020.
//

#ifndef FINALPROJECT_BOARD_H
#define FINALPROJECT_BOARD_H

#include <Box2D/Box2D.h>
#include <cinder/gl/gl.h>
#include <vector>
class Board {
 public:
  Board(b2World* world);


  float GetFrontLine() {return front_line;}

  float GetBackLine() {return back_line;}

  float GetUpperSideLine() {return ky;}

  float GetLowerSideLine() {return ky + kHeight;}

  void Display();

  cinder::vec2 GetTeePoint() {return tee;}


 private:
  float kx = 0;
  float ky = 200;
  float kWidth = 2070;
  float kHeight = 400;
  float front_line = 100.0f;
  float back_line = 1800.0f;

  const cinder::vec2 tee = {1600, 400};
};
#endif  // FINALPROJECT_BOARD_H
