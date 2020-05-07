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

  /// winner state enum of who won
  enum class WinnerState{
    RedWins,
    YellowWins,
    NoWinner
  };

  /// get the winner of the game
  engine::WinnerState GetWinner();

  /// check if a rock is sliding and should make a sound
  bool ShouldPlaySound();

  /// Check if contact happened
  bool IsContact();

  /// check if rock went outside vertically
  void CheckOutOfBoundsVertical();

  /// check if rock went outside horizontally
  void CheckOutOfBoundsHorizontal();

  /// update the right rocks to the house list accordingly
  void UpdateRocksInHouse();

  /// remove rock from rocks vector
  void RemoveRock(Rock* remove);

  /// Update the variable numlauches
  void UpdateNumLaunches();

  /// check if all the rocks are slowed down and should move on
  bool AreAllRocksAreSlowed();

  /// update the number of rocks red could launch
  void UpdateRedLeft();

  /// update the number of rocks yellow could launch
  void UpdateYellowLeft();

  /// get the score of the winner using the two vectors of rocks in the house for each team
  int GetWinnerScore();

  /// get the closest rock from the tee from a list of rocks
  Rock* GetClosestRockFromTee(std::vector<Rock*> list);

  /// check if all rocks are stopped
  bool AreAllRocksAreStopped();


  /// the below are getters and setters for different files and classes

  bool GetIsLaunched() {return is_launched;}
  void SetIsYPointSelected(bool input);
  bool GetIsYPointSelected() {return is_y_point_selected;}
  void SetYPoint(int input);
  std::vector<Rock*> GetRedRocksInHouse() {return rocks_in_house_red;}
  std::vector<Rock*> GetYellowRocksInHouse() {return rocks_in_house_yellow;}
  Rock* GetCurrentRock() {return current_rock;}
  bool GetIsSetOver() {return is_set_over;}
  void SetUseOB(bool input);
  bool GetIsLastRockLaunched() {return is_last_rock_launched;}
  int GetRedLeft() {return red_left;}
  int GetYellowLeft() {return yellow_left;}
  void SetWinner();
  bool GetIsRedTurn() {return is_red_turn;}



 private:
  /// the rocks that are on the board
  std::vector<Rock*> rocks;

  /// the rocks in the house for red team
  std::vector<Rock*> rocks_in_house_red;

  /// the rocks in the house for red team
  std::vector<Rock*> rocks_in_house_yellow;

  /// if the current rock is launched
  bool is_launched;

  /// if its reds turn
  bool is_red_turn;

  /// the current rock at the moment
  Rock* current_rock;
  b2World* world;

  /// the set y_point of the user
  float y_point;

  /// if the y point is selected by the user
  bool is_y_point_selected;
  Board* board;

  /// the number of rocks launched
  int num_launches;

  /// the turns each user gets
  int kTurns = 5;

  /// the winner of the game
  WinnerState winner;
  bool is_set_over;

  /// if the game is using out of bounds
  bool use_ob;

  /// number of rocks that are left to be launched
  int red_left;
  int yellow_left;

  /// if the current rock is the last rock to be launched
  bool is_last_rock_launched;
};

#endif  // FINALPROJECT_ENGINE_H
