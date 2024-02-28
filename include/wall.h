#ifndef		__H__WALL__
#define		__H__WALL__

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/physics.h"
#include <box2d/box2d.h>

class Wall : public Drawable{//only drawable because it is a static body
	public:
		Wall(PhysicsWorld*, float, float, float, float);
		~Wall();
		void draw(SDL_Renderer* renderer) override;
		float flip(float);
		void setBody(b2Body*);
		b2BodyDef* getBodyDef();
		b2Body* getBody();
	private:
		int x_vel;
		int y_vel;
		b2Body* body;
		b2BodyDef* bodyDef;
};

#endif

