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
  explicit engine(b2World* input_world, Board* input_board);

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

  void CheckOutOfBoundsHorizontal();

  void RemoveRock(Rock* remove);

  void CheckOutOfBoundsVertical();

  void UpdateRocksInHouse();

  void GetRocksInHouse();

  int GetWinnerScore();

  Rock* GetClosestRockFromTee(std::vector<Rock*> list);


  bool GetIsGameOver() {return is_game_over;}

  void UpdateNumLaunches();

  bool AllRocksAreStopped();

  void SetUseOB(bool input);

  enum class WinnerState{
    RedWins,
    YellowWins,
    NoWinner
  };

  engine::WinnerState GetWinner();

 private:
  std::vector<Rock*> rocks;
  std::vector<Rock*> rocks_in_house_red;
  std::vector<Rock*> rocks_in_house_other;
  bool is_launched;
  bool is_red_turn;
  Rock* current_rock;
  b2World* world;
  float y_point;
  bool is_y_point_selected;
  Board* board;
  int num_launches;
  int kTurns = 3;
  WinnerState winner;
  bool is_game_over;
  bool use_ob;
};

#endif  // FINALPROJECT_ENGINE_H
