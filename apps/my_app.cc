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
b2Body* m_body;
float timeStep = 1.0f / 20.0f;

MyApp::MyApp() { }

void MyApp::setup() {
  b2Vec2 gravity(0.0f, 0);
  m_world = new b2World(gravity);

  rock = new Rock(m_world, 50);
//  board = new Board(m_world);
}

void MyApp::update() {
  rock->updateFriction();
   for( int i = 0; i < 10; ++i )
    m_world->Step( 1 / 30.0f, 10, 10 );
}

void MyApp::draw() {
  cinder::gl::enableAlphaBlending();
  cinder::gl::clear();

  cinder::gl::color(1, 1, 1);

//  cinder::gl::drawSolidRect(cinder::Rectf{board->getLeftTopCorner().x, board->getLeftTopCorner().y,
//                                          board->getRightBottomCorner().x, board->getRightBottomCorner().y});

  cinder::gl::color(1, 0.5, 1);
  cinder::gl::drawSolidCircle({rock->getBody()->GetPosition().x, rock->getBody()->GetPosition().y}, 50);
}

void MyApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_UP:
      rock->getBody()->ApplyLinearImpulse({50, 10},
                                          rock->getBody()->GetWorldCenter());
  }
}
void MyApp::mouseDown(cinder::app::MouseEvent event) {
  rock->m_body->ApplyLinearImpulse({500000, 0},
                              rock->m_body->GetWorldCenter());
}
}  // namespace myapp
