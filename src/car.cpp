#include "car.h"

#include <box2d/box2d.h>

#include <iostream>
#include <random>
#include <string>

#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "mathfu/vector.h"

Car::Car(PhysicsWorld* physics, bool isRed) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 10);

    Car::isRed = isRed;
    Car::accel = 0;

    // Image is 76x38, where Box2D expects 100px=1m. Probably make the hitbox slightly smaller.
    if (isRed) {
        loadImage("./assets/redcar.png");
    } else {
        loadImage("./assets/greencar.png");
    }

    // Need a body definition before we can make a body
    bodyDef = new b2BodyDef();
    bodyDef->type = b2_dynamicBody;
    if (isRed) {
        // Red player will start on the left.
        bodyDef->position.Set(0, -3.9f);
    } else {
        bodyDef->position.Set(8.25f, -3.9f);
    }
	bodyDef->linearDamping = 0.3f;
	bodyDef->angularDamping = 0.3f;
    // Physics engine makes the body for us and returns a pointer to it
    body = physics->addBody(bodyDef);
    // Need a shape
    b2PolygonShape carShape;
    /* Each of these is half the width/height, /100 for meters, then lowered slightly
     * for a more generous hitbox. */
    carShape.SetAsBox(0.30f, 0.11f);
    // Must apply a fixture.  Fixes shape and other properties to it.
    b2FixtureDef carFixture;
    carFixture.shape = &carShape;
    carFixture.density = 1.0f;
    carFixture.friction = 0.9f;
    carFixture.restitution = 0.6f;
    // Make the fixture.
    body->CreateFixture(&carFixture);
}

Car::~Car() {
    delete bodyDef;
}

void Car::setBody(b2Body* body) {
    this->body = body;
}

b2BodyDef* Car::getBodyDef() {
    return bodyDef;
}

void Car::update(double delta) {
    // std::cout << body->GetPosition().x << ", " << body->GetPosition().y << std::endl;
    b2Vec2 currentVelocity = body->GetLinearVelocity();
    auto events = Engine::getEvents();
	if (events.empty()) {
		accel = 0;
	}
    for (auto event = events.begin(); event != events.end(); ++event) {
        if (event->type == SDL_KEYDOWN) {
            if ((Car::isRed && event->key.keysym.sym == SDLK_w) || ((!Car::isRed) && event->key.keysym.sym == SDLK_UP)) {
				// Thanks to this example I found online for this code: https://github.com/RonakFabian/Asteroids-SFML-Box2D/blob/main/Asteroids/Rocket.cpp
                float X = 1 * sin(body->GetAngle());
				float Y = 1 * -cos(body->GetAngle());
				body->ApplyForceToCenter(b2Vec2(X, Y), true);
            } else if ((Car::isRed && event->key.keysym.sym == SDLK_s) || ((!Car::isRed) && event->key.keysym.sym == SDLK_DOWN)) {
				body->ApplyForceToCenter({-1,0}, true);
			} else {
				accel = 0;
			}

            if ((Car::isRed && event->key.keysym.sym == SDLK_a) || ((!Car::isRed) && event->key.keysym.sym == SDLK_LEFT)) {
				body->ApplyTorque(-ROTATE, true);
            } else if ((Car::isRed && event->key.keysym.sym == SDLK_d) || ((!Car::isRed) && event->key.keysym.sym == SDLK_RIGHT)) {
				body->ApplyTorque(ROTATE, true);
			}

			// Original code.
            /* if (event->key.keysym.sym == SDLK_SPACE) {
                b2Vec2 up(0.0f, 1.0f);
                b2Vec2 pos = body->GetPosition();
                pos.x += 0.1;
                body->ApplyLinearImpulse(up, pos, true);
                body->ApplyTorque(10.0f, true);
            } */
        }
    }
}

void Car::draw(SDL_Renderer* renderer) {
    SDL_Rect dest;
    b2Vec2 pos = body->GetPosition();
    dest.x = pos.x * 100;
    dest.y = pos.y * -1 * 100;
    dest.w = rect.w;
    dest.h = rect.h;
    float angle = body->GetAngle();
    // std::cout << dest.x << ", " << dest.y << std::endl;
    int result = SDL_RenderCopyEx(renderer, texture, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
    if (result != 0) {
        std::cout << SDL_GetError() << std::endl;
    }
}

b2Body* Car::getBody() {
    return body;
}
