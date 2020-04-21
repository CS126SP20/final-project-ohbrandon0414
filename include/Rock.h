//
// Created by ohbra on 4/20/2020.
//

#ifndef FINALPROJECT_ROCK_H
#define FINALPROJECT_ROCK_H

#include <Box2D/Box2D.h>

#include <glm/fwd.hpp>

class Rock {
   public:
    b2Body* m_body;
    float m_radius;
    Rock(b2World* world, float radius);
    b2Body* getBody() {return m_body;}
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();

  void updateFriction();
    float getX() {return m_body->GetPosition().x;}
    float getY() {return m_body->GetPosition().y;}

   private:
    b2Fixture* fixture = nullptr;


};

#endif  // FINALPROJECT_ROCK_H
