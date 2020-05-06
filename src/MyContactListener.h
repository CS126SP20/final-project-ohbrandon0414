//
// Created by ohbra on 5/5/2020.
//

#ifndef FINALPROJECT_MYCONTACTLISTENER_H
#define FINALPROJECT_MYCONTACTLISTENER_H
#include <Box2D/Box2D.h>
#include <Rock.h>
class MyContactListener : public b2ContactListener{
  void BeginContact(b2Contact* contact) {

    //check if fixture A was a ball
    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if ( bodyUserData )
      static_cast<Rock*>( bodyUserData )->startContact();

    //check if fixture B was a ball
    bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if ( bodyUserData )
      static_cast<Rock*>( bodyUserData )->startContact();

  }

  void EndContact(b2Contact* contact) {

    //check if fixture A was a ball
    void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
    if ( bodyUserData )
      static_cast<Rock*>( bodyUserData )->endContact();

    //check if fixture B was a ball
    bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
    if ( bodyUserData )
      static_cast<Rock*>( bodyUserData )->endContact();

  }
};

#endif  // FINALPROJECT_MYCONTACTLISTENER_H
