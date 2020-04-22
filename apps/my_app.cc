// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>
#include <ObjectArray.h>


#include <vector>
#include <Rock.h>
#include <Board.h>
#include <engine.h>
namespace myapp {

using cinder::app::KeyEvent;

int power;
int selected_power;
b2World* m_world;
Board* board;
engine* _engine;
Rock* currentRock;

MyApp::MyApp() { }

void MyApp::setup() {
  // basic set up
  b2Vec2 gravity(0.0f, 0);
  m_world = new b2World(gravity);
  _engine = new engine(m_world);
  board = new Board(m_world);
}

void MyApp::update() {
  // updates the current rock with in this file
  currentRock = _engine->GetCurrentRock();
   for( int i = 0; i < 5; ++i ){
     m_world->Step( 1 / 100.0f, 10, 10 );
   }
   // engine executes a steo
  _engine->Step();

}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  // takes care of the power gage.
  power = static_cast<int>((ci::app::getElapsedSeconds()) * 9.9);
  power = power % 10;
  std::string str = std::to_string(power + 1);
  PrintText(str, {500, 500}, {100,70});
  // prints out the selected power.
  std::string str2 = std::to_string(selected_power + 1);
  PrintText(str2, {500, 500}, {200,70});

  // displays the board and all the rocks
  board->Display();
  for(Rock* temp: _engine->GetRocks()) {
    temp->Display();
  }}


void MyApp::keyDown(KeyEvent event) {
}

/// mouse click controls the power gage.
void MyApp::mouseDown(cinder::app::MouseEvent event) {
  selected_power = power;
  currentRock->GetBody()->ApplyLinearImpulse({static_cast<float32>((power + 1) * 9999999) , 0},
                                      currentRock->GetBody()->GetWorldCenter());
//  std::string str = std::to_string(power);
  _engine->SetLaunched(true);
}

/// creates a textbox for the power
void MyApp::PrintText(const std::string& text, const glm::ivec2& size, const glm::vec2& loc) {
  cinder::gl::color(1,1,0.5);

  auto box = cinder::TextBox()
      .alignment(cinder::TextBox::CENTER)
      .font(cinder::Font("Arial", 100))
      .size(size)
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}
}  // namespace myapp
