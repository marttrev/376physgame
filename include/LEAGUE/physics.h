#ifndef		__H__PHYSICS__
#define		__H__PHYSICS__

#include <box2d/box2d.h>
#include <LEAGUE/common_data_structures.h>
#include "game_listener.h"

class PhysicsWorld : public Updateable {
	public:
		PhysicsWorld(b2Vec2 gravity);
		~PhysicsWorld();
		b2Body* addBody(b2BodyDef* bodyDefinition);
		b2Joint* addJoint(b2JointDef* jointDefinition);
		void update(double delta);
	private:
		b2Vec2 gravity;
		b2World world;
		GameListener* listener;
		int velocityIterations = 6;
		int positionIterations = 2;
};

#endif
