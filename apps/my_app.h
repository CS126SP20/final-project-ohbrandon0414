// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <Location.h>


namespace myapp {

class MyApp : public cinder::app::App {
 public:
  MyApp();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
  void mouseDown(cinder::app::MouseEvent) override;

 private:
  // update the attributes at the moment
  void UpdateAttributes();

  /// draw the right attributes need to be shown
  void DrawAttributes();

  /// draw the game over screen
  void DrawGameOver();

  /// draw the options page
  void DrawStartScreen();

  /// draw score at the bottom for each sets.
  void DrawScore();

  /// apply the score after each set
  void ApplyScore();

  /// user gets to choose the options
  void ChooseOptions(const cinder::app::MouseEvent& event);

  /// private method to print texts.
  void PrintText(const std::string& text, const cinder::ivec2& size,
                 const cinder::vec2& loc, bool changer);

  void StepBot();


  bool is_angle_set;
  bool should_show_angle;
  float angle_y_point;
  bool should_show_placement;
  bool is_set_over;
  bool is_start_screen;
  bool is_using_key;
  bool is_using_mouse;

  int kRange = 100;
  bool is_using_ob;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
