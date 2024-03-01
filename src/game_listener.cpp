#include <iostream>
#include <box2d/box2d.h>
#include "user_data.h"
#include "game_listener.h"

void GameListener::BeginContact(b2Contact *contact) {
    std::cout << "Contacted." << std::endl;
    // Outdated, but helpful in making sense of this: https://www.youtube.com/watch?v=pJ_M_fACtB8
    b2Body* firstBody = contact->GetFixtureA()->GetBody();
    b2Body* secondBody = contact->GetFixtureB()->GetBody();
    struct UserData* firstUData = reinterpret_cast<UserData*>(firstBody->GetUserData().pointer);
    struct UserData* secondUData = reinterpret_cast<UserData*>(secondBody->GetUserData().pointer);

    // If something touches a damaging object (the spikes), reduce that object's HP.
    if (firstUData->isDamaging) {
        secondUData->hp--;
    } else if (secondUData->isDamaging) {
        firstUData->hp--;
    }
}
