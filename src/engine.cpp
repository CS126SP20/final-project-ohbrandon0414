//
// Created by ohbra on 4/21/2020.
//

#include "engine.h"
#include "Board.h"
#include <algorithm>
#include <Location.h>


engine::engine(Board* input_board) {
  board = input_board;
  world = board->GetWorld();
  current_rock = nullptr;
  is_launched = false;
  is_red_turn = true;
  is_y_point_selected = false;
  num_launches = 0;
  is_set_over = false;
  winner = WinnerState::NoWinner;
  use_ob = true;
  red_left = kTurns;
  yellow_left = kTurns;
  is_last_rock_launched = false;
}

void engine::CreateRock(Rock* rock) {
  current_rock = rock;
  rocks.push_back(rock);
}

void engine::SetIsLaunched(bool input) {
  current_rock->SetIsLaunched(input);
  // flips the turn.
  if(input) {
    is_red_turn = !is_red_turn;
  }
  is_launched = input;
}

void engine::Step() {
  UpdateRocksInHouse();
  if (num_launches >= (2 * kTurns) &&
      (current_rock == nullptr || AllRocksAreStopped())) {
    is_set_over = true;
    SetWinner();
    return;
  }
  if (current_rock != nullptr && use_ob) {
    CheckOutOfBoundsHorizontal();
    CheckOutOfBoundsVertical();
  }
  if (num_launches >= (2 * kTurns)) {
    is_last_rock_launched = true;
    return;
  }
  if((current_rock == nullptr && is_y_point_selected)
      || (is_y_point_selected && is_launched && AllRocksAreSlowed())) {

    Rock* rock = new Rock(world, {board->GetFrontLine(),  y_point}, is_red_turn);
    CreateRock(rock);
    if(rock->IsRed()) {
      UpdateRedLeft();
    } else {
      UpdateYellowLeft();
    }
    is_launched = false;

  }
}

void engine::SetIsYPointSelected(bool input) {
  is_y_point_selected = input;
}
void engine::SetYPoint(int input){
  y_point = input;
}

void engine::CheckOutOfBoundsHorizontal() {
  for (Rock* temp: rocks) {
    float x_pos = temp->GetPosition().GetX();
    if (x_pos >= board->GetBackLine() || (temp->IsLaunched()
             && temp->IsSlowedDown()
            && x_pos < board->GetTeePoint().GetX() - 2 * board->GetHouseRadius())) {
      RemoveRock(temp);
      break;
    }
  }
}

void engine::CheckOutOfBoundsVertical() {
  for (Rock* temp: rocks) {
    float y_pos = temp->GetPosition().GetY();
    float radius = temp->GetRadius();

    if (y_pos + radius >= board->GetLowerSideLine()
        || y_pos - radius <= board->GetUpperSideLine()) {
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

int engine::GetWinnerScore() {
  int count = 0;

  if (winner == WinnerState::RedWins) {
    Rock* yellow = GetClosestRockFromTee(rocks_in_house_yellow);
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
      return rocks_in_house_yellow.size();
    }
    float r_dis = red->GetPosition().distance(board->GetTeePoint());
    for(Rock* rock: rocks_in_house_yellow) {
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
        rocks_in_house_yellow.push_back(rock);
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

void engine::SetWinner() {
  if(rocks_in_house_red.empty() && rocks_in_house_yellow.empty()) {
    winner = engine::WinnerState::NoWinner;
    return;
  }
  if (rocks_in_house_yellow.empty()) {
    winner = engine::WinnerState::RedWins;
    return;
  }
  if (rocks_in_house_red.empty()) {
    winner = engine::WinnerState::YellowWins;
    return;
  }

  if(GetClosestRockFromTee(rocks)->IsRed()) {
    winner = engine::WinnerState::RedWins;
    return;
  } else {
    winner = engine::WinnerState::YellowWins;
    return;
  }
}
engine::WinnerState engine::GetWinner() {
  return winner;
}

void engine::UpdateNumLaunches() {
  num_launches++;
}
bool engine::AllRocksAreSlowed() {
  for (Rock* rock: rocks) {
    if (!rock->IsSlowedDown()) {
      return false;
    }
  }
  return true;
}

void engine::Reset() {
  current_rock = nullptr;
  for(Rock* temp: rocks) {
    delete temp;
  }
  rocks.clear();
  num_launches = 0;
  is_set_over = false;
  yellow_left = kTurns;
  red_left = kTurns;
  rocks_in_house_red.clear();
  rocks_in_house_yellow.clear();
  is_last_rock_launched = false;
  is_red_turn = !is_red_turn;
}
void engine::SetUseOB(bool input) {
  use_ob = input;
}
void engine::UpdateRedLeft() {
  red_left--;
}
void engine::UpdateYellowLeft() {
  yellow_left--;
}
bool engine::AllRocksAreStopped() {
  for (Rock* rock: rocks) {
    if (!rock->IsCompletelyStopped()) {
      return false;
    }
  }
  return true;
}

