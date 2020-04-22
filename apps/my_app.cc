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

  rock = new Rock(m_world, 30, {100, 500});
  board = new Board(m_world);
}

void MyApp::update() {
   for( int i = 0; i < 10; ++i ){
     m_world->Step( 1 / 30.0f, 10, 10 );
   }
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  drawBoard();
  drawRock();
}


void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
      rock->getBody()->ApplyLinearImpulse({50, 10},
                                          rock->getBody()->GetWorldCenter());
  }
}
void MyApp::mouseDown(cinder::app::MouseEvent event) {
  rock->getBody()->ApplyLinearImpulse({500000, 0},
                              rock->getBody()->GetWorldCenter());
}
void MyApp::drawRock() {
  cinder::gl::color(1, 0.5, 1);
  for(Rock* temp: rocks) {
    cinder::gl::drawSolidCircle({temp->getBody()->GetPosition().x, temp->getBody()->GetPosition().y}, temp->getRadius());
  }
}
void MyApp::drawBoard() {
  cinder::gl::color(1, 1, 1);
  cinder::gl::drawSolidRect(cinder::Rectf{board->getLeftTopCorner().x, board->getLeftTopCorner().y,
                                          board->getRightBottomCorner().x, board->getRightBottomCorner().y});
}
}  // namespace myapp
