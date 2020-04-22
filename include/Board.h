//
// Created by ohbra on 4/20/2020.
//

#ifndef FINALPROJECT_BOARD_H
#define FINALPROJECT_BOARD_H

#include <Box2D/Box2D.h>
#include <vector>
class Board {
 public:
  Board(b2World* world);

  /// gets the left top corner of the board.
  b2Vec2 getLeftTopCorner() {return {kx, ky};}

  /// gets the bottom right corner of the board.
  b2Vec2 getRightBottomCorner() {return {kx + kWidth, ky + kHeight * 2};}
  void Display();

 private:
  float kx = 0;
  float ky = 200;
  float kWidth = 1800;
  float kHeight = 200;
};
#endif  // FINALPROJECT_BOARD_H
