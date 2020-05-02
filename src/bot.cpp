//
// Created by ohbra on 5/1/2020.
//

#include "bot.h"

bot::bot(Board *input_board, engine *input_engine) {
  board = input_board;
  engine_ = input_engine;
  x_placement = board->GetFrontLine();
  y_placement = board->GetTeePoint().GetY();
}

b2Vec2 bot::GetForce() {
  Rock* rock = engine_->GetClosestRockFromTee(engine_->GetRocks());
  float x = abs(board->GetTeePoint().GetX() - x_placement);
  float y = board->GetTeePoint().GetY() - y_placement;
  if(rock->IsRed()) {
    x = abs(rock->GetPosition().GetX() - x_placement);
    y = rock->GetPosition().GetY() - y_placement;
  }
  return {x * 1000, y * 1000};
}

int bot::GetPlacement() {
  return y_placement;
}
