//
// Created by ohbra on 4/21/2020.
//

#include "engine.h"

engine::engine(b2World* input) {
  world = input;
  turn_is_over = false;
  is_launched = false;
  is_red_turn = true;
  is_y_point_selected = false;
}

void engine::CreateRock(Rock* rock) {
  current_rock = rock;
  rocks.push_back(rock);
}

void engine::SetIsLaunched(bool input) {
  is_launched = input;
}

void engine::Step() {
  if((rocks.size() == 0 && is_y_point_selected)
      || (is_y_point_selected && is_launched && current_rock->IsStopped())) {

    Rock* rock = new Rock(world, {100.0f, (float32) y_point}, is_red_turn);
    CreateRock(rock);
    is_launched = false;

    // flips the turn.
    is_red_turn = !is_red_turn;
  }
}

void engine::Reset() {}

void engine::SetIsYPointSelected(bool input) {
  is_y_point_selected = input;
}
void engine::SetYPoint(int input) {
  y_point = input;
}

