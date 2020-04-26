//
// Created by ohbra on 4/21/2020.
//

#ifndef FINALPROJECT_TEAM_H
#define FINALPROJECT_TEAM_H

#include <Rock.h>
#include <engine.h>
#include <vector>

class team {
 public:
  void addRock(Rock* input);
  int GetScore(cinder::vec2 vec);
  float GetClosestRock();
 private:
  std::vector<Rock*> rocks;
  int score;
};

#endif  // FINALPROJECT_TEAM_H
