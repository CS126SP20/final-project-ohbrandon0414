//
// Created by ohbra on 4/21/2020.
//

#ifndef FINALPROJECT_ENGINE_H
#define FINALPROJECT_ENGINE_H
#include <Rock.h>
#include <vector>

class engine {
 public:
  engine(b2World* input);
  void CreateRock(Rock* rock);
  std::vector<Rock*> GetRocks(){return rocks;}
  void Reset();
  void Step();
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
