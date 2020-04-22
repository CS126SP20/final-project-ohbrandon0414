//
// Created by ohbra on 4/21/2020.
//

#include "engine.h"

engine::engine(b2World* input) {
  world = input;
  turn_is_over = false;
  is_launched = false;
  is_red_turn = true;
  Rock* rock = new Rock(world, {100.0f, 450.0f}, is_red_turn);
  is_red_turn = false;
  CreateRock(rock);
}

void engine::CreateRock(Rock* rock) {
  current_rock = rock;
  rocks.push_back(rock);
}

void engine::SetLaunched(bool input) {
  is_launched = input;
}


void engine::Step() {
  if(is_launched && current_rock->IsStopped()) {
    Rock* rock = new Rock(world, {100.0f, 450.0f}, is_red_turn);
    CreateRock(rock);
    is_launched = false;

    if(is_red_turn) {
      is_red_turn = false;
    } else {
      is_red_turn = true;
    }
  }
}

void engine::Reset() {}

