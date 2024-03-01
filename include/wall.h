#ifndef		__H__WALL__
#define		__H__WALL__

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/physics.h"
#include "user_data.h"
#include <box2d/box2d.h>

class Wall : public Drawable {//only drawable because it is a static body
	public:
		Wall(const char*, PhysicsWorld*, float, float, float, float);
		~Wall();
		struct UserData userData;
		void draw(SDL_Renderer* renderer) override;
		float flip(float);
		void setBody(b2Body*);
		b2BodyDef* getBodyDef();
		b2Body* getBody();
	private:
		float halfHeight;
		float halfWidth;
		b2Body* body;
		b2BodyDef* bodyDef;
};

#endif

