#ifndef		__H__CAR__
#define		__H__CAR__

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/physics.h"
#include <box2d/box2d.h>

class Car : public Drawable, public Updateable {
	public:
		Car(PhysicsWorld*, bool);
		~Car();
		void draw(SDL_Renderer* renderer) override;
		void update(double delta) override;
		float flip(float);
		void setBody(b2Body*);
		b2BodyDef* getBodyDef();
		b2Body* getBody();
	private:
		
		int x_vel;
		int y_vel;
		bool isRed;
		bool isUp;
		bool isDown;
		bool isLeft;
		bool isRight;
		b2Body* body;
		b2BodyDef* bodyDef;
		void processInputs();
		float convertAngle(float initAngle);
};

#endif
