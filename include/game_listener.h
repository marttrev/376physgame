#ifndef __H__CAR_GAME_CONTACT_LISTENER__
#define __H__CAR_GAME_CONTACT_LISTENER__

#include <box2d/box2d.h>

class GameListener : public b2ContactListener {
    public:
        void BeginContact(b2Contact*);
};

#endif