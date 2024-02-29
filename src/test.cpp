#include <SDL2/SDL.h>
#include <iostream>
#include <unistd.h>

#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "ball.h"
#include "car.h"
#include "wall.h"
#include "caution_wall.h"

int main(int argc, char** argv){
	int opt;
	while((opt = getopt(argc, argv, "h")) != -1){

	}
	Scene scene;
	Engine* engine = Engine::getInstance();
	PhysicsWorld physics(b2Vec2(0.0, 0.0));

	Car* redCar = new Car(&physics, true);
	Car* greenCar = new Car(&physics, false);

	Wall* topWall = new Wall("./assets/1024x100spikes.png", &physics, 5.12f, 0.5f, 5.12f, 0.2f);
	Wall* bottomWall = new Wall("./assets/1024x100spikes.png", &physics, 5.12f, 0.5f, 5.12f, -7.4f);
	Wall* leftWall = new Wall("./assets/100x768spikes.png", &physics, 0.5f, 3.84f, -0.5f, -3.84f);
	Wall* rightWall = new Wall("./assets/100x768spikes.png", &physics, 0.5f, 3.84f, 9.7f, -3.84f);

	CautionWall* leftCaution = new CautionWall("./assets/30x150wall.png", &physics, 0.15f, 0.75f, 4.0f, -4.0f);

	scene.addUpdateable(physics);
	scene.addUpdateable(*redCar);
	scene.addUpdateable(*greenCar);
	
	scene.addDrawable(*redCar);
	scene.addDrawable(*greenCar);
	scene.addDrawable(*topWall);
	scene.addDrawable(*bottomWall);
	scene.addDrawable(*leftWall);
	scene.addDrawable(*rightWall);
	scene.addDrawable(*leftCaution);

	engine->core_loop(scene);
	engine->shutdown();
}
