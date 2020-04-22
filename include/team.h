//
// Created by ohbra on 4/21/2020.
//

#ifndef FINALPROJECT_TEAM_H
#define FINALPROJECT_TEAM_H

#include <Rock.h>
#include <vector>

class team {
 public:
  void addRock(Rock input);
  int getScore();
 private:
  std::vector<Rock> rocks;
  int score;
};

#endif  // FINALPROJECT_TEAM_H
