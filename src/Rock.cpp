//
// Created by ohbra on 4/20/2020.
//


#include "Rock.h"
#include <glm/fwd.hpp>
#include <Box2D/Box2D.h>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>


Rock::Rock(b2World* world, b2Vec2 loc, bool red) {
  is_contact = false;
  m_body = nullptr;
  is_red = red;
  is_launched = false;

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
  myFixtureDef.density = 5;
  myFixtureDef.restitution = 0.5f;
  m_body->CreateFixture(&myFixtureDef);

  m_numContacts = 0;
  m_body->SetUserData( this ); //set this Ball object in the body's user data
}

void Rock::Display() {
  // outer part
  cinder::gl::color(0.5,0.5,0.5);
  cinder::gl::drawSolidCircle({m_body->GetPosition().x, m_body->GetPosition().y}, m_radius);

  cinder::gl::color(1,1,0);
  if (is_red) {
    cinder::gl::color(1,0,0);
  }
  cinder::gl::drawSolidCircle({m_body->GetPosition().x, m_body->GetPosition().y}, m_radius / 2);
}

bool Rock::IsSlowedDown() {
  return m_body->GetLinearVelocity().x <= 2 && m_body->GetLinearVelocity().y <= 2;
}
Location Rock::GetPosition() {
  return {m_body->GetPosition().x, m_body->GetPosition().y};
}
Rock::~Rock() {
  m_body->GetWorld()->DestroyBody(m_body);
}
bool Rock::IsCompletelyStopped() {
  if(m_body->GetLinearVelocity().x <= 0.5
      && m_body->GetLinearVelocity().y <= 0.5 )  {
    m_body->SetLinearVelocity(b2Vec2_zero);
    return true;
  }
  return false;
}
bool Rock::IsContact() {return is_contact;}
void Rock::SetIsContact(bool input) {
  is_contact = input;
}
