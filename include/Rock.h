//
// Created by ohbra on 4/20/2020.
//

#ifndef FINALPROJECT_ROCK_H
#define FINALPROJECT_ROCK_H

#include <Box2D/Box2D.h>

#include <glm/fwd.hpp>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>

class Rock {
   public:
    Rock(b2World* world, float radius, b2Vec2 loc, bool red);
    b2Body* GetBody() {return m_body;}
    float GetRadius() {return m_radius;}
    bool IsRed() {return is_red;}
    void Display();

   private:
    b2Body* m_body;
    bool is_red;
    float m_radius;
};

#endif  // FINALPROJECT_ROCK_H
