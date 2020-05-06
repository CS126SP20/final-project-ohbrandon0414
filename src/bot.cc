//
// Created by ohbra on 5/1/2020.
//

#include "bot.h"

bot::bot(Board *input_board, engine *input_engine) {
  board = input_board;
  engine_ = input_engine;
  x_placement = board->GetFrontLine();

  y_placement = rand() % 200 + 300;
  guarded = false;
}

b2Vec2 bot::GetForce() {
  // the default x and y for the target
  float x = abs(board->GetTeePoint().GetX() - x_placement);
  float y = board->GetTeePoint().GetY() - y_placement;


  if (IsGuarded()) {
    return {x * 1000, y * 1000} ;
  }
  if(engine_->GetYellowLeft() == 0) {
    x = board->GetTeePoint().GetX() - x_placement;
  } else{
    /// the house is empty
    if (engine_->GetRedRocksInHouse().empty() && engine_->GetYellowRocksInHouse().empty()) {
      x = board->GetTeePoint().GetX() - x_placement - 50 - board->GetHouseRadius();
    } else {
      // the closest rock to the tee
      Rock* rock = engine_->GetClosestRockFromTee(engine_->GetRocks());

      /// no red rocks but yellow rocks in the house
      if (engine_->GetRedRocksInHouse().empty()) {
        x = rock->GetPosition().GetX() - 50 - x_placement;
        y = rock->GetPosition().GetY() - y_placement;
      } else {
        if(rock->IsRed()&& abs(rock->GetPosition().GetY() - board->GetTeePoint().GetY())
                           < board->GetHouseRadius() / 2) {
          x = rock->GetPosition().GetX() - x_placement + 100;
          y = rock->GetPosition().GetY() - y_placement;
        }
        else {
          x = board->GetTeePoint().GetX() - x_placement - 50 - board->GetHouseRadius();
        }
      }
    }
  }


  return {x * 1000, y * 1000};
}

int bot::GetPlacement() {
  return y_placement;
}

bool bot::IsGuarded() {
  for(Rock* rock: engine_->GetRocks()) {
    float x = rock->GetPosition().GetX();
    float y = rock->GetPosition().GetY();
    if(x <= 1450 && x >= 1250 && y <= 500 && y >= 300) {
      return true;
    }
  }
  return false;
}
