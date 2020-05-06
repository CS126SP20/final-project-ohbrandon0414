//
// Created by ohbra on 4/20/2020.
//

#ifndef FINALPROJECT_ROCK_H
#define FINALPROJECT_ROCK_H

#include <Box2D/Box2D.h>

#include <glm/fwd.hpp>
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Location.h>

class Rock {
   public:
    /// constructor to set up the basics for each rock
    Rock(b2World* world, b2Vec2 loc, bool red);

    /// gets the physical body of the rock.
    b2Body* GetBody() {return m_body;}

    /// gets the radius of the rock.
    float GetRadius() {return m_radius;}

    /// checks if the rock is on the red team.
    bool IsRed() {return is_red;}

    /// draws the rock on the screen.
    void Display();

    /// checks if the rock was already launched
    bool IsLaunched() {return is_launched;}

    void SetIsLaunched(bool input) {is_launched = input;}

    /// checks if the rocks completely stopped.
    bool IsSlowedDown();

    bool IsCompletelyStopped();

    void SetIsContact(bool input);

    Location GetPosition();

    ~Rock();


  //new implementation for contact state change
    void startContact() {
      is_contact = true;
      m_numContacts++;
    }
    void endContact() { m_numContacts--; }
    bool IsContact();
   private:
    int m_numContacts;
    b2Body* m_body;
    bool is_red;
    // radius of the rock
    float m_radius = 25;
    bool is_launched;
    bool is_contact;
};

#endif  // FINALPROJECT_ROCK_H
