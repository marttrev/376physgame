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
#include "road.h"
#include "HPText.h"

int main(int argc, char** argv){
	int opt;
	while((opt = getopt(argc, argv, "h")) != -1){

	}
	Scene scene;
	Engine* engine = Engine::getInstance();
	PhysicsWorld physics(b2Vec2(0.0, 0.0));

	Road* road = new Road();

	Car* redCar = new Car(&physics, true);
	Car* greenCar = new Car(&physics, false);

	HPText* redHP = new HPText(redCar, {255, 0, 0}, 50, 20);
	HPText* greenHP = new HPText(greenCar, {0, 255, 0}, 900, 20);

	Wall* topWall = new Wall("./assets/1024x100spikes.png", &physics, 5.12f, 0.5f, 5.12f, 0.2f);
	Wall* bottomWall = new Wall("./assets/1024x100spikes.png", &physics, 5.12f, 0.5f, 5.12f, -7.4f);
	Wall* leftWall = new Wall("./assets/100x768spikes.png", &physics, 0.5f, 3.84f, -0.4f, -3.84f);
	Wall* rightWall = new Wall("./assets/100x768spikes.png", &physics, 0.5f, 3.84f, 9.8f, -3.84f);

	CautionWall* leftCaution = new CautionWall("./assets/30x150wall.png", &physics, 0.15f, 0.75f, 2.9f, -3.8f);
	CautionWall* rightCaution = new CautionWall("./assets/30x150wall.png", &physics, 0.15f, 0.75f, 6.8f, -3.8f);
	CautionWall* topLeftCaution = new CautionWall("./assets/150x30wall.png", &physics, 0.75f, 0.15f, 2.0f, -1.5f);
	CautionWall* topCenterCaution = new CautionWall("./assets/150x30wall.png", &physics, 0.75f, 0.15f, 4.8f, -1.5f);
	CautionWall* topRightCaution = new CautionWall("./assets/150x30wall.png", &physics, 0.75f, 0.15f, 7.6f, -1.5f);
	CautionWall* bottomLeftCaution = new CautionWall("./assets/150x30wall.png", &physics, 0.75f, 0.15f, 2.0f, -6.0f);
	CautionWall* bottomCenterCaution = new CautionWall("./assets/150x30wall.png", &physics, 0.75f, 0.15f, 4.8f, -6.0f);
	CautionWall* bottomRightCaution = new CautionWall("./assets/150x30wall.png", &physics, 0.75f, 0.15f, 7.6f, -6.0f);


	scene.addUpdateable(physics);
	scene.addUpdateable(*redCar);
	scene.addUpdateable(*greenCar);
	
	scene.addDrawable(*road);
	scene.addDrawable(*redCar);
	scene.addDrawable(*greenCar);
	scene.addDrawable(*topWall);
	scene.addDrawable(*bottomWall);
	scene.addDrawable(*leftWall);
	scene.addDrawable(*rightWall);
	scene.addDrawable(*leftCaution);
	scene.addDrawable(*rightCaution);
	scene.addDrawable(*topLeftCaution);
	scene.addDrawable(*topCenterCaution);
	scene.addDrawable(*topRightCaution);
	scene.addDrawable(*bottomLeftCaution);
	scene.addDrawable(*bottomCenterCaution);
	scene.addDrawable(*bottomRightCaution);
	scene.addDrawable(*redHP);
	scene.addDrawable(*greenHP);

	engine->core_loop(scene);
	engine->shutdown();
}
