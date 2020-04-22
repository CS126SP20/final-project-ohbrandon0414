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
   for( int i = 0; i < 10; ++i ){
     m_world->Step( 1 / 100.0f, 10, 10 );
   }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  board->Display();
  for(Rock* temp: rocks) {
    temp->Display();
  }}


void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
      rock->GetBody()->ApplyLinearImpulse({50, 10},
                                          rock->GetBody()->GetWorldCenter());
  }
}
void MyApp::mouseDown(cinder::app::MouseEvent event) {
  rock->GetBody()->ApplyLinearImpulse({9999999, 0},
                                      rock->GetBody()->GetWorldCenter());
}
}  // namespace myapp
