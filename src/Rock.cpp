//
// Created by ohbra on 4/20/2020.
//


#include "Rock.h"
#include <glm/fwd.hpp>
#include <Box2D/Box2D.h>

Rock::Rock(b2World* world, float radius, b2Vec2 loc) {
  m_body = nullptr;
  m_radius = radius;

  //set up dynamic body, store in class variable
  b2BodyDef myBodyDef;
  myBodyDef.type = b2_dynamicBody;
  myBodyDef.linearDamping = 0.1f;
  myBodyDef.position.Set(loc.x, loc.y);
  m_body = world->CreateBody(&myBodyDef);

  //add circle fixture
  b2CircleShape circleShape;
  circleShape.m_p.Set(0, 0);
  circleShape.m_radius = m_radius; //use class variable
  b2FixtureDef myFixtureDef;
  myFixtureDef.shape = &circleShape;
  myFixtureDef.density = 1;
  myFixtureDef.friction = 0.3f;
  myFixtureDef.restitution = 0.5f;
  m_body->CreateFixture(&myFixtureDef);
}

