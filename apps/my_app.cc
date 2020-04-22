// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "my_app.h"

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Box2D/Box2D.h>
#include <ObjectArray.h>


#include <vector>
#include <Rock.h>
#include <Board.h>
namespace myapp {

using cinder::app::KeyEvent;

int num;
std::vector<Rock*> rocks;
b2World* m_world;
Rock* rock;
Board* board;

MyApp::MyApp() { }

void MyApp::setup() {
  b2Vec2 gravity(0.0f, 0);
  m_world = new b2World(gravity);

  rock = new Rock(m_world, kRadius, {100, 450},true);
  Rock* stat = new Rock(m_world, kRadius, {1000, 440}, false);
  rocks.push_back(stat);
  rocks.push_back(rock);
  board = new Board(m_world);
}

void MyApp::update() {
   for( int i = 0; i < 5; ++i ){
     m_world->Step( 1 / 100.0f, 10, 10 );
   }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  num = static_cast<int>((ci::app::getElapsedSeconds()) * 9.9);
  num = num % 10;
  std::string str = std::to_string(num);
  PrintText(str, {500, 50}, {150,30});

  board->Display();
  for(Rock* temp: rocks) {
    temp->Display();
  }}


void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_s:
      rock->GetBody()->ApplyLinearImpulse({9999999, 0},
                                          rock->GetBody()->GetWorldCenter());
  }
}
void MyApp::mouseDown(cinder::app::MouseEvent event) {

  rock->GetBody()->ApplyLinearImpulse({static_cast<float32>(num * 9999999), 0},
                                      rock->GetBody()->GetWorldCenter());
}
void MyApp::PrintText(const std::string& text, const glm::ivec2& size, const glm::vec2& loc) {
  cinder::gl::color(1,1,0.5);


  auto box = cinder::TextBox()
      .alignment(cinder::TextBox::CENTER)
      .font(cinder::Font("Arial", 30))
      .size(size)
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp = {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);
}
}  // namespace myapp
