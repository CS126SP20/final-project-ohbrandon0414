//
// Created by ohbra on 4/20/2020.
//

#ifndef FINALPROJECT_ROCK_H
#define FINALPROJECT_ROCK_H

#include <Box2D/Box2D.h>

#include <glm/fwd.hpp>

class Rock {
   public:
    Rock(b2World* world, float radius, b2Vec2 loc);
    b2Body* getBody() {return m_body;}
    float getRadius() {return m_radius;}

   private:
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    b2Body* m_body;
    float m_radius;
};

#endif  // FINALPROJECT_ROCK_H
