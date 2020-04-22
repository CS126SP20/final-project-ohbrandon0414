//
// Created by ohbra on 4/21/2020.
//

#include "engine.h"

engine::engine(b2World* input) {
  world = input;
  Rock* rock = new Rock(world, 25, {100.0f, 450.0f}, true);
  CreateRock(rock);
  current_rock = rock;
}

void engine::CreateRock(Rock* rock) {
  rocks.push_back(rock);
}


void engine::Reset() {}
void engine::SetLaunched(bool input) {
  is_launched = input;
}
void engine::Step() {
  if(is_launched && current_rock->IsStopped()) {
    Rock* rock = new Rock(world, 25, {100.0f, 450.0f}, false);
    CreateRock(rock);
    current_rock = rock;
  }
}
