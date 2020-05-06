//
// Created by ohbra on 4/21/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H
#include <Rock.h>
#include <Board.h>
#include <vector>

class engine {
 public:
  engine(Board* input_board);

  /// creates a rock with in the game
  void CreateRock(Rock* rock);

  /// gets the list of the rocks in a vector form
  std::vector<Rock*> GetRocks(){return rocks;}

  /// resets the game when its over
  void Reset();

  /// a method to execute to the next step of the game
  void Step();

  /// a setter for the mouse click
  void SetIsLaunched(bool input);

  bool GetIsLaunched() {return is_launched;}

  void SetIsYPointSelected(bool input);

  bool GetIsYPointSelected() {return is_y_point_selected;}

  void SetYPoint(int input);

  Rock* GetCurrentRock() {return current_rock;}

  int GetWinnerScore();

  Rock* GetClosestRockFromTee(std::vector<Rock*> list);

  bool GetIsSetOver() {return is_set_over;}

  void UpdateNumLaunches();

  bool AllRocksAreSlowed();

  void SetUseOB(bool input);

  bool AllRocksAreStopped();

  bool IsLastRockLaunched() {return is_last_rock_launched;}

  void UpdateRedLeft();

  void UpdateYellowLeft();

  int GetRedLeft() {return red_left;}

  int GetYellowLeft() {return yellow_left;}

  enum class WinnerState{
    RedWins,
    YellowWins,
    NoWinner
  };

  void SetWinner();

  bool GetIsRedTurn() {return is_red_turn;}

  engine::WinnerState GetWinner();

  std::vector<Rock*> GetRedRocksInHouse() {return rocks_in_house_red;}

  std::vector<Rock*> GetYellowRocksInHouse() {return rocks_in_house_yellow;}

  bool ShouldPlaySound();

  bool IsContact();

 private:

  void CheckOutOfBoundsHorizontal();

  void RemoveRock(Rock* remove);

  void CheckOutOfBoundsVertical();

  void UpdateRocksInHouse();


  std::vector<Rock*> rocks;
  std::vector<Rock*> rocks_in_house_red;
  std::vector<Rock*> rocks_in_house_yellow;
  bool is_launched;
  bool is_red_turn;
  Rock* current_rock;
  b2World* world;
  float y_point;
  bool is_y_point_selected;
  Board* board;
  int num_launches;
  int kTurns = 5;
  WinnerState winner;
  bool is_set_over;
  bool use_ob;
  int red_left;
  int yellow_left;
  bool is_last_rock_launched;
};

#endif  // FINALPROJECT_ENGINE_H
