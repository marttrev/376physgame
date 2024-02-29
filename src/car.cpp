#include "car.h"

#include <box2d/box2d.h>

#include <cmath>
#include <iostream>
#include <random>
#include <string>

#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "mathfu/vector.h"

const int MAGNITUDE = 100;

Car::Car(PhysicsWorld* physics, bool isRed) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 10);

    Car::isRed = isRed;

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
    bodyDef->angularDamping = 0.5f;
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
    for (auto event = events.begin(); event != events.end(); ++event) {
        if (event->type == SDL_KEYDOWN) {
            if ((Car::isRed && event->key.keysym.sym == SDLK_w) || ((!Car::isRed) && event->key.keysym.sym == SDLK_UP)) {
                float angle = convertAngle(body->GetAngle());
                /* Get the vector from the (arbitrary) magnitude and current facing angle. I'd forgotten how to do this. Sourced from here:
                 * https://www.khanacademy.org/math/precalculus/x9e81a4f98389efdf:vectors/x9e81a4f98389efdf:component-form/v/vector-components-from-magnitude-and-direction */
                float X = (MAGNITUDE * delta) * std::cos(angle * (M_PI / 180));
                float Y = (MAGNITUDE * delta) * std::sin(angle * (M_PI / 180));
                body->ApplyForceToCenter(b2Vec2(X, Y), true);
            } else if ((Car::isRed && event->key.keysym.sym == SDLK_s) || ((!Car::isRed) && event->key.keysym.sym == SDLK_DOWN)) {
                float angle = convertAngle(body->GetAngle());
                /* Get the vector from the (arbitrary) magnitude and current facing angle. I'd forgotten how to do this. Sourced from here:
                 * https://www.khanacademy.org/math/precalculus/x9e81a4f98389efdf:vectors/x9e81a4f98389efdf:component-form/v/vector-components-from-magnitude-and-direction */
                float X = (MAGNITUDE * delta) * std::cos(angle * (M_PI / 180));
                float Y = (MAGNITUDE * delta) * std::sin(angle * (M_PI / 180));
                body->ApplyForceToCenter(-b2Vec2(X, Y), true);
            }

            if ((Car::isRed && event->key.keysym.sym == SDLK_a) || ((!Car::isRed) && event->key.keysym.sym == SDLK_LEFT)) {
                // We want to utilize the SetTransform() function, and pass the same position but update the angle in radians
                // ApplyTorque isn't the function we want to be using
                body->ApplyAngularImpulse(-MAGNITUDE/100 * delta, true);
            } else if ((Car::isRed && event->key.keysym.sym == SDLK_d) || ((!Car::isRed) && event->key.keysym.sym == SDLK_RIGHT)) {
                body->ApplyAngularImpulse(MAGNITUDE/100 * delta, true);
            }

            if (event->key.keysym.sym == SDLK_i) {
                float angle = fmod(body->GetAngle(), 360);
                if (angle > 0) {
                    // Clockwise angles should go backwards from 360 but are positive.
                    angle = 360 - angle;
                } else {
                    // Counter-clockwise angles should be positive but are negative.
                    angle *= -1;
                }

                float posX = body->GetPosition().x;
                float posY = body->GetPosition().y;
                std::cout << "Angle: " << angle << std::endl;
                std::cout << "Vector: <" << (100 * delta * cos(angle * (180 / M_PI))) << ", " << (100 * delta * sin(angle * (180 / M_PI))) << ">" << std::endl
                          << std::endl;
                std::cout << "Position: " << posX << ", " << posY << std::endl;
            }
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

float Car::convertAngle(float initAngle) {
    /* Mod by 360 just to make things more readable. Don't think it's needed. Mod by floats is an invalid operation,
     * hence why we use the fmod function instead. */
    float angle = fmod(initAngle, 360);

    // Convert to usable values
    if (angle > 0) {
        // Clockwise angles should go backwards from 360 but are positive.
        angle = 360 - angle;
    } else {
        // Counter-clockwise angles should be positive but are negative.
        angle *= -1;
    }

    return angle;
}