//
// Created by ohbra on 4/20/2020.
//

#ifndef FINALPROJECT_BOARD_H
#define FINALPROJECT_BOARD_H

#include <Box2D/Box2D.h>
#include <vector>
class Board {
 public:
  b2Body* m_body;
  Board(b2World* world);
  b2Body* getBody() {return m_body;}
  b2Fixture* getFixture() {return fixture;}
  b2Vec2 getLeftTopCorner() {return {kx, ky};}
  b2Vec2 getRightBottomCorner() {return {kx + kWidth, ky + kHeight * 2};}
  float getX() {return m_body->GetPosition().x;}
  float getY() {return m_body->GetPosition().y;}

 private:
  float kx = 0;
  float ky = 200;
  float kWidth = 1800;
  float kHeight = 200;
  b2Fixture* fixture = nullptr;
};
#endif  // FINALPROJECT_BOARD_H
