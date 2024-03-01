#ifndef		__H__CAUTION_WALL__
#define		__H__CAUTION_WALL__

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/physics.h"
#include "user_data.h"
#include <box2d/box2d.h>

class CautionWall : public Drawable{//only drawable because it is a static body
	public:
		CautionWall(const char*, PhysicsWorld*, float, float, float, float);
		~CautionWall();
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

