//
// Created by ohbra on 4/21/2020.
//

#include "engine.h"
#include "Board.h"
#include <algorithm>
#include <Location.h>


engine::engine(b2World* input_world, Board* input_board) {
  world = input_world;
  current_rock = nullptr;
  turn_is_over = false;
  is_launched = false;
  is_red_turn = true;
  is_y_point_selected = false;
  board = input_board;
  num_launches = 0;
  is_game_over = false;
}

void engine::CreateRock(Rock* rock) {
  current_rock = rock;
  rocks.push_back(rock);
  num_launches++;
}

void engine::SetIsLaunched(bool input) {
  is_launched = input;
}

void engine::Step() {
  if (num_launches >= 2 * kTurns) {
    is_game_over = true;
    return;
  }
  if (current_rock != nullptr) {
    CheckOutOfBoundsHorizontal();
    CheckOutOfBoundsVertical();
  }
  if((current_rock == nullptr && is_y_point_selected)
      || (is_y_point_selected && is_launched && current_rock->IsStopped())) {

    Rock* rock = new Rock(world, {board->GetFrontLine(),  y_point}, is_red_turn);
    CreateRock(rock);
    is_launched = false;

    // flips the turn.
    is_red_turn = !is_red_turn;
  }
}

void engine::SetIsYPointSelected(bool input) {
  is_y_point_selected = input;
}
void engine::SetYPoint(int input) {
  y_point = input;
}

void engine::CheckOutOfBoundsHorizontal() {
  for (Rock* temp: rocks) {
    if (temp->GetPosition().GetX() >= board->GetBackLine()) {
      RemoveRock(temp);
      break;
    }
  }
}

void engine::CheckOutOfBoundsVertical() {
  for (Rock* temp: rocks) {
    float y_pos = temp->GetPosition().GetY();
    float radius = temp->GetRadius() + 10;

    if (y_pos + radius >= board->GetLowerSideLine() || y_pos - radius <= board->GetUpperSideLine()) {
      RemoveRock(temp);
      break;
    }
  }
}



void engine::Reset() {}
void engine::RemoveRock(Rock* rock) {
  if (rock == current_rock) {
    current_rock = nullptr;
  }
  rocks.erase(std::remove(rocks.begin(), rocks.end(), rock), rocks.end());
  delete (rock);
}

int engine::GetWinnerScore() {
  b2Vec2 min = {FLT_MAX, FLT_MAX};
  float distance = FLT_MAX;
  for (Rock* rock: rocks) {
    b2Vec2 pos = rock->GetBody()->GetPosition();
//    if (distance <= pos)
  }
  return 1;
}
