//
// Created by ohbra on 4/20/2020.
//


#include "Rock.h"
#include <glm/fwd.hpp>
#include <Box2D/Box2D.h>


Rock::Rock(b2World* world, float radius) {
  m_body = nullptr;
  m_radius = radius;

  //set up dynamic body, store in class variable
  b2BodyDef myBodyDef;
  myBodyDef.type = b2_dynamicBody;
  myBodyDef.position.Set(100, 500);
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
b2Vec2 Rock::getLateralVelocity() {
  b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(1,0) );
  return b2Dot( currentRightNormal, m_body->GetLinearVelocity() ) * currentRightNormal;
}
b2Vec2 Rock::getForwardVelocity() {
  b2Vec2 currentRightNormal = m_body->GetWorldVector( b2Vec2(0,1) );
  return b2Dot( currentRightNormal, m_body->GetLinearVelocity() ) * currentRightNormal;
}
void Rock::updateFriction() {
  b2Vec2 impulse = m_body->GetMass() * -getLateralVelocity();
  m_body->ApplyLinearImpulse(impulse, m_body->GetWorldCenter());

  m_body->ApplyAngularImpulse( 0.1f * m_body->GetInertia() * -m_body->GetAngularVelocity() );

  b2Vec2 currentForwardNormal = getForwardVelocity();
  float currentForwardSpeed = currentForwardNormal.Normalize();
  float dragForceMagnitude = -2 * currentForwardSpeed;
  m_body->ApplyForce( dragForceMagnitude * currentForwardNormal, m_body->GetWorldCenter() );


}
