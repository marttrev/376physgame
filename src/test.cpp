#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "ball.h"
#include "car.h"

int main(int argc, char** argv){
	int opt;
	while((opt = getopt(argc, argv, "h")) != -1){

	}
	Scene scene;
	Engine* engine = Engine::getInstance();
	PhysicsWorld physics(b2Vec2(0.0, 0.0));

	b2BodyDef groundDef;
	groundDef.position.Set(0.0f, -8.2f);
	groundDef.type=b2_staticBody;
	b2Body* ground = physics.addBody(&groundDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 1.0f);
	ground->CreateFixture(&groundBox, 1.0f);


	b2BodyDef leftDef;
	leftDef.position.Set(-1.5f, -7.6f);
	leftDef.type=b2_staticBody;
	b2Body* left = physics.addBody(&leftDef);
	b2PolygonShape leftBox;
	leftBox.SetAsBox(1.0f, 50.0f);
	left->CreateFixture(&leftBox, 1.0f);

	b2BodyDef rightDef;
	rightDef.position.Set(10.6f, -7.6f);
	rightDef.type=b2_staticBody;
	b2Body* right = physics.addBody(&rightDef);
	b2PolygonShape rightBox;
	rightBox.SetAsBox(1.0f, 50.0f);
	right->CreateFixture(&rightBox, 1.0f);

	b2BodyDef ceilDef;
	ceilDef.position.Set(0.0f, 0.8f);
	ceilDef.type=b2_staticBody;
	b2Body* ceil = physics.addBody(&ceilDef);
	b2PolygonShape ceilBox;
	ceilBox.SetAsBox(50.0f, 1.0f);
	ceil->CreateFixture(&ceilBox, 1.0f);

	/* for(int i=0; i<500; ++i){
		Ball* b = new Ball(&physics);
		scene.addUpdateable(*b);
		scene.addDrawable(*b);
	}
 */
	//Car* redCar = new Car(&physics, true);
	Car* greenCar = new Car(&physics, false);
	//scene.addUpdateable(*redCar);
	scene.addUpdateable(*greenCar);
	//scene.addDrawable(*redCar);
	scene.addDrawable(*greenCar);

	scene.addUpdateable(physics);
	engine->core_loop(scene);
	engine->shutdown();
}
