//
// Created by ohbra on 4/21/2020.
//

#include "engine.h"

engine::engine(b2World* input) {
  world = input;
  turn_is_over = false;
  is_launched = false;
  Rock* rock = new Rock(world, 25, {100.0f, 450.0f}, true);
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
    Rock* rock = new Rock(world, 25, {100.0f, 450.0f}, false);
    CreateRock(rock);
    is_launched = false;
  }
}

void engine::Reset() {}

