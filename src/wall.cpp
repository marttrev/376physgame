#include "mathfu/vector.h"
#include "wall.h"
#include "LEAGUE/physics.h"
#include "LEAGUE/engine.h"
#include <iostream>
#include <string>
#include <box2d/box2d.h>
#include <random>

Wall::Wall(PhysicsWorld* physics, float width, float height, float xpos, float ypos){

	loadImage("./assets/wall.png");//image from https://mushahroomali.itch.io/pixel-art-stone-floorwall
	// Need a body definition before we can make a body
	bodyDef = new b2BodyDef();
	bodyDef->type = b2_staticBody;//define the wall as a static body
	bodyDef->position.Set(xpos, ypos);
	// Physics engine makes the body for us and returns a pointer to it
	body = physics->addBody(bodyDef);
	// Need a shape
	b2PolygonShape wallShape;
	wallShape.SetAsBox(width/2, height/2);//need to divide width and height by 2
	// Must apply a fixture.  Fixes shape and other properties to it.
	b2FixtureDef wallFixture;
	wallFixture.shape = &wallShape;
	wallFixture.density = 1.0f;
	wallFixture.friction = 0.3f;
	wallFixture.restitution = 0.3f;
	// Make the fixture.
	body->CreateFixture(&wallFixture);
}

Wall::~Wall(){
	delete bodyDef;
}

void Wall::setBody(b2Body* body){
	this->body = body;
}

b2BodyDef* Wall::getBodyDef(){
	return bodyDef;
}


void Wall::draw(SDL_Renderer* renderer){
	SDL_Rect dest;
	b2Vec2 pos = body->GetPosition();
	dest.x = pos.x * 100;
	dest.y = pos.y * -1 * 100;
	dest.w = rect.w;
	dest.h = rect.h;
	float angle = body->GetAngle();
	//std::cout << dest.x << ", " << dest.y << std::endl;
	int result = SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
	if(result != 0){
		std::cout << SDL_GetError() << std::endl;
	}
}

b2Body* Wall::getBody(){
	return body;
}

