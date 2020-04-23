//
// Created by ohbra on 4/20/2020.
//

#include "Board.h"

#include <engine.h>
#include <cinder/gl/wrapper.h>

Board::Board(b2World *world) {
  //a static body
  b2BodyDef myBodyDef;
  myBodyDef.type = b2_staticBody;
  myBodyDef.position.Set(0, 0);
  b2Body* staticBody = world->CreateBody(&myBodyDef);

  //shape definition
  b2PolygonShape polygonShape;

  //fixture definition
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &polygonShape;

  //add four walls to the static body
  polygonShape.SetAsBox( kWidth, 1, b2Vec2(kx, ky), 0);//ceiling
  staticBody->CreateFixture(&myFixtureDef);
  polygonShape.SetAsBox( kWidth, 1, b2Vec2(kx, ky + kHeight * 2), 0);//ground
  staticBody->CreateFixture(&myFixtureDef);
  polygonShape.SetAsBox( 1, kHeight, b2Vec2(kx, ky + kHeight), 0);//left wall
  staticBody->CreateFixture(&myFixtureDef);
  polygonShape.SetAsBox( 1, kHeight, b2Vec2(kx + kWidth, ky + kHeight), 0);//right wall
  staticBody->CreateFixture(&myFixtureDef);
}
void Board::Display() {
  cinder::gl::color(0.9f, 0.9f, 0.9f);
  cinder::gl::drawSolidRect(cinder::Rectf{kx, ky,kx + kWidth, ky + kHeight * 2});

  cinder::gl::color(1,0,0);
  cinder::gl::drawSolidRect(cinder::Rectf{100, 200, 125, 600});

  cinder::gl::color(0,0,1);
  cinder::gl::drawSolidCircle(tee, 150);

  cinder::gl::color(1,1,1);
  cinder::gl::drawSolidCircle(tee, 100);

  cinder::gl::color(0.9,0,0);
  cinder::gl::drawSolidCircle(tee, 50);

  cinder::gl::color(1,1,1);
  cinder::gl::drawSolidCircle(tee, 20);
}
