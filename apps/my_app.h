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
  void UpdateAttributes();
  void DrawAttributes();
 private:
  void PrintText(const std::string& text, const cinder::ivec2& size,
                 const cinder::vec2& loc);
  bool angle_is_selected;
  bool should_show_angle;
  float angle_y_point;
  bool should_show_placement;
};

}  // namespace myapp

#endif  // FINALPROJECT_APPS_MYAPP_H_
