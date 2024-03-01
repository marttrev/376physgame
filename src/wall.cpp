#include "wall.h"
#include "user_data.h"

#include <box2d/box2d.h>

#include <iostream>
#include <random>
#include <string>

#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "mathfu/vector.h"

Wall::Wall(const char* path, PhysicsWorld* physics, float halfWidth, float halfHeight, float centerX, float centerY) {
    Wall::halfHeight = halfHeight * 100;
    Wall::halfWidth = halfWidth * 100;

    // For collision
    Wall::userData = {true, 0};

    // Need a body definition before we can make a body
    bodyDef = new b2BodyDef();
    bodyDef->type = b2_staticBody;  // define the wall as a static body
    bodyDef->position.Set(centerX, centerY);
    // To detect collision
    bodyDef->userData.pointer = reinterpret_cast<uintptr_t>(&userData);
    // Physics engine makes the body for us and returns a pointer to it
    body = physics->addBody(bodyDef);
    // Need a shape
    b2PolygonShape wallShape;
    wallShape.SetAsBox(halfWidth, halfHeight);  // need to divide width and height by 2
    // Must apply a fixture.  Fixes shape and other properties to it.
    b2FixtureDef wallFixture;
    wallFixture.shape = &wallShape;
    wallFixture.density = 1.0f;
    wallFixture.friction = 0.3f;
    wallFixture.restitution = 0.0f;
    // Make the fixture.
    body->CreateFixture(&wallFixture);
    loadImage(path);
}

Wall::~Wall() {
    delete bodyDef;
}

void Wall::setBody(b2Body* body) {
    this->body = body;
}

b2BodyDef* Wall::getBodyDef() {
    return bodyDef;
}

void Wall::draw(SDL_Renderer* renderer) {
    SDL_Rect dest;
    b2Vec2 pos = body->GetPosition();
    /* I have no idea why this scales like this but it does so we're doing this I guess. Optimized for 100x768
	 * and 1024x100 pixel walls, no clue if this will work with other sizes because I've not tried yet. */
    if (Wall::halfHeight > Wall::halfWidth) {
		// Vertical scaling.
		dest.x = pos.x * 100;
		dest.y = (pos.y * -1 * 100) - Wall::halfHeight;
    } else {
        // Horizontal scaling.
		dest.x = (pos.x * 100) - Wall::halfWidth;
        dest.y = (pos.y * -1 * 100) - (Wall::halfHeight / 2);
    }
    dest.w = rect.w;
    dest.h = rect.h;
    float angle = body->GetAngle();
    // std::cout << dest.x << ", " << dest.y << std::endl;
    int result = SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
    if (result != 0) {
        std::cout << SDL_GetError() << std::endl;
    }
}

b2Body* Wall::getBody() {
    return body;
}