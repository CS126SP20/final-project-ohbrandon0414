//
// Created by ohbra on 4/21/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H
#include <Rock.h>
#include <vector>

class engine {
 public:
  explicit engine(b2World* input);

  /// creates a rock with in the game
  void CreateRock(Rock* rock);

  /// gets the list of the rocks in a vector form
  std::vector<Rock*> GetRocks(){return rocks;}

  /// resets the game when its over
  void Reset();

  /// a method to execute to the next step of the game
  void Step();

  /// a setter for the mouse click
  void SetLaunched(bool input);

  Rock* GetCurrentRock() {return current_rock;}

 private:
  std::vector<Rock*> rocks;
  bool turn_is_over;
  bool is_launched;
  Rock* current_rock;
  b2World* world;
};

#endif  // FINALPROJECT_ENGINE_H
