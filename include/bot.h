//
// Created by ohbra on 5/1/2020.
//

#ifndef FINALPROJECT_BOT_H
#define FINALPROJECT_BOT_H

#include <engine.h>
#include <Board.h>
#include <Box2D/Box2D.h>


class bot {
 public:
  bot(Board* input_board, engine* input_engine);
  int GetPlacement();
  b2Vec2 GetForce();
  bool IsGuarded();

 private:
  int y_placement;
  int x_placement;
  Board* board;
  engine* engine_;
  bool guarded;
};

#endif  // FINALPROJECT_BOT_H
