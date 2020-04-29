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
  is_launched = false;
  is_red_turn = true;
  is_y_point_selected = false;
  board = input_board;
  num_launches = 0;
  is_game_over = false;
  winner = WinnerState::NoWinner;
  use_ob = true;
}

void engine::CreateRock(Rock* rock) {
  current_rock = rock;
  rocks.push_back(rock);
}

void engine::SetIsLaunched(bool input) {
  is_launched = input;
}

void engine::Step() {
  if (num_launches >= (2 * kTurns) &&
      (current_rock == nullptr || current_rock->IsCompletelyStopped())) {
    is_game_over = true;
    UpdateRocksInHouse();
    winner = GetWinner();
    return;
  }
  if (current_rock != nullptr && use_ob) {
    CheckOutOfBoundsHorizontal();
    CheckOutOfBoundsVertical();
  }
  if((current_rock == nullptr && is_y_point_selected)
      || (is_y_point_selected && is_launched && AllRocksAreStopped())) {

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

void engine::RemoveRock(Rock* rock) {
  if (rock == current_rock) {
    current_rock = nullptr;
  }
  rocks.erase(std::remove(rocks.begin(),
                          rocks.end(), rock), rocks.end());
  delete (rock);
}


void engine::GetRocksInHouse() {

}
int engine::GetWinnerScore() {
  int count = 0;

  if (winner == WinnerState::RedWins) {
    Rock* yellow = GetClosestRockFromTee(rocks_in_house_other);
    if (yellow == nullptr) {
      return rocks_in_house_red.size();
    }
    float y_dis = yellow->GetPosition().distance(board->GetTeePoint());
    for (Rock* rock: rocks_in_house_red) {
      float rock_dis = rock->GetPosition().distance(board->GetTeePoint());
      if(rock_dis <= y_dis) {
        count++;
      }
    }
    return count;
  }
  if (winner == WinnerState::YellowWins){
    Rock* red = GetClosestRockFromTee(rocks_in_house_red);
    if (red == nullptr) {
      return rocks_in_house_other.size();
    }
    float r_dis = red->GetPosition().distance(board->GetTeePoint());
    for(Rock* rock: rocks_in_house_other) {
      float rock_dis = rock->GetPosition().distance(board->GetTeePoint());
      if(rock_dis <= r_dis) {
        count++;
      }
    }
    return count;
  }
  return 0;
}
void engine::UpdateRocksInHouse() {
  for (Rock* rock: rocks) {
    float distance = rock->GetPosition().distance(board->GetTeePoint());
    if (distance <= board->GetHouseRadius()) {
      if(rock->IsRed()) {
        rocks_in_house_red.push_back(rock);
      } else {
        rocks_in_house_other.push_back(rock);
      }
    }
  }
}
Rock* engine::GetClosestRockFromTee(std::vector<Rock*> list) {
  float min = FLT_MAX;
  Rock* winning_rock = nullptr;
  if (list.empty()) {
    return winning_rock;
  }
  for (Rock* rock: list) {
    float distance = rock->GetPosition().distance(board->GetTeePoint());
    if (distance <= min) {
      min = distance;
      winning_rock = rock;
    }
  }
  return winning_rock;
}
engine::WinnerState engine::GetWinner() {
  if(rocks_in_house_red.empty() && rocks_in_house_other.empty()) {
    return engine::WinnerState::NoWinner;
  }
  if (rocks_in_house_other.empty()) {
    return engine::WinnerState::RedWins;
  }
  if (rocks_in_house_red.empty()) {
    return engine::WinnerState::YellowWins;
  }

  if(GetClosestRockFromTee(rocks)->IsRed()) {
    return engine::WinnerState::RedWins;
  } else {
    return engine::WinnerState::YellowWins;
  }
}

void engine::UpdateNumLaunches() {
  num_launches++;
}
bool engine::AllRocksAreStopped() {
  for (Rock* rock: rocks) {
    if (!rock->IsSlowedDown()) {
      return false;
    }
  }
  return true;
}

void engine::Reset() {
  for(Rock* temp: rocks) {
    delete temp;
  }
  rocks.clear();
  num_launches = 0;
  is_game_over = false;
}
void engine::SetUseOB(bool input) {
  use_ob = input;
}
