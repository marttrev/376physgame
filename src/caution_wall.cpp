#include "caution_wall.h"

#include <box2d/box2d.h>

#include <iostream>
#include <random>
#include <string>

#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "mathfu/vector.h"

CautionWall::CautionWall(const char* path, PhysicsWorld* physics, float halfWidth, float halfHeight, float centerX, float centerY) {
    CautionWall::halfHeight = halfHeight * 100;
    CautionWall::halfWidth = halfWidth * 100;

    // These walls are *just walls,* so they are non-damaging. Arbitrary hp value.
    CautionWall::userData = {false, false, 0};

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
    wallShape.SetAsBox(halfWidth, halfHeight); 
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

CautionWall::~CautionWall() {
    delete bodyDef;
}

void CautionWall::setBody(b2Body* body) {
    this->body = body;
}

b2BodyDef* CautionWall::getBodyDef() {
    return bodyDef;
}

void CautionWall::draw(SDL_Renderer* renderer) {
    SDL_Rect dest;
    b2Vec2 pos = body->GetPosition();
    /* Despite my best efforts, these do not draw appropriately to the hitbox, so there's some *weird* scaling
     * going on here to try and dial it in. If this doesn't make sense to you, don't worry, it doesn't make sense
     * to me either, I just brute-forced these numbers. */
    if (CautionWall::halfHeight > CautionWall::halfWidth) {
		// Vertical scaling.
		dest.x = pos.x * 100 + CautionWall::halfWidth;
		dest.y = (pos.y * -1 * 100) - (CautionWall::halfHeight - 15);
    } else {
        // Horizontal scaling.
		dest.x = (pos.x * 100) - 35;
        dest.y = (pos.y * -1 * 100) + 5;
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

b2Body* CautionWall::getBody() {
    return body;
}
