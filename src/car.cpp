#include "car.h"

#include <box2d/box2d.h>

#include <cmath>
#include <iostream>
#include <random>
#include <string>

#include "LEAGUE/engine.h"
#include "LEAGUE/physics.h"
#include "mathfu/vector.h"
#include "user_data.h"

const int ACCEL_MAGNITUDE = 15;
const int TORQUE_MAGNITUDE = 70;

Car::Car(PhysicsWorld* physics, bool isRed) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 10);

    // Initialize values.
    Car::isRed = isRed;
    // These will be used for tracking inputs.
    Car::isUp = false;
    Car::isDown = false;
    Car::isLeft = false;
    Car::isRight = false;
    Car::isAlive = true;

    // Set 5hp. This time it matters.
    Car::userData = {false, true, 5};

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
        bodyDef->position.Set(1.25, -3.9f);
    } else {
        bodyDef->position.Set(8.25f, -3.9f);
    }
    // Since we're doing top-down, *this* is really what simulates friction.
    bodyDef->linearDamping = 0.6f;
    bodyDef->angularDamping = 1.0f;
    // To detect collision, this will get pulled out by our listener.
    bodyDef->userData.pointer = reinterpret_cast<uintptr_t>(&userData);
    // Physics engine makes the body for us and returns a pointer to it
    body = physics->addBody(bodyDef);
    if (!isRed) {
        // Docs say the angle should be in radians. It wants degrees. Stupid but whatever.
        body->SetTransform(body->GetPosition(), 180);
    }
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
    carFixture.restitution = 0.5f;
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
    if (userData.hp > 0) {
        Car::processInputs();
        if (Car::isUp) {
            // Get the angle the car is facing.
            float angle = convertAngle(body->GetAngle());
            /* Get the vector from the (arbitrary) magnitude and current facing angle. I'd forgotten how to do this. Sourced from here:
             * https://www.khanacademy.org/math/precalculus/x9e81a4f98389efdf:vectors/x9e81a4f98389efdf:component-form/v/vector-components-from-magnitude-and-direction */
            float X = (ACCEL_MAGNITUDE * delta) * std::cos(angle * (M_PI / 180));
            float Y = (ACCEL_MAGNITUDE * delta) * std::sin(angle * (M_PI / 180));
            // Move forward.
            body->ApplyForceToCenter(b2Vec2(X, Y), true);
        } else if (Car::isDown) {
            // Get the angle the car is facing.
            float angle = convertAngle(body->GetAngle());
            /* Get the vector from the (arbitrary) magnitude and current facing angle. I'd forgotten how to do this. Sourced from here:
             * https://www.khanacademy.org/math/precalculus/x9e81a4f98389efdf:vectors/x9e81a4f98389efdf:component-form/v/vector-components-from-magnitude-and-direction */
            float X = (ACCEL_MAGNITUDE * delta) * std::cos(angle * (M_PI / 180));
            float Y = (ACCEL_MAGNITUDE * delta) * std::sin(angle * (M_PI / 180));
            // Negating the vector so we move backward.
            body->ApplyForceToCenter(-b2Vec2(X, Y), true);
        }

        if (Car::isLeft) {
            body->ApplyTorque(-TORQUE_MAGNITUDE * delta, true);
        } else if (Car::isRight) {
            body->ApplyTorque(TORQUE_MAGNITUDE * delta, true);
        }
    } else if (userData.hp <= 0 && Car::isAlive) {
        loadImage("./assets/gravestone.png");
        // Flip the gravestone upright.
        body->SetTransform(body->GetPosition(), 0);
        
        // set alive to false and hp to 0 (in case it somehow became negative)
        Car::isAlive = false;
        userData.takesDamage = false;
        userData.hp = 0;
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

UserData Car::getUserData(){
    return this->userData;
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

void Car::processInputs() {
    int numkeys = 0;
    SDL_PumpEvents();
    const Uint8* keystate = SDL_GetKeyboardState(&numkeys);

    // continuous-response keys
    Car::isUp = (Car::isRed && keystate[SDL_SCANCODE_W]) || ((!Car::isRed) && keystate[SDL_SCANCODE_UP]);
    Car::isDown = (Car::isRed && keystate[SDL_SCANCODE_S]) || ((!Car::isRed) && keystate[SDL_SCANCODE_DOWN]);
    Car::isLeft = (Car::isRed && keystate[SDL_SCANCODE_A]) || ((!Car::isRed) && keystate[SDL_SCANCODE_LEFT]);
    Car::isRight = (Car::isRed && keystate[SDL_SCANCODE_D]) || ((!Car::isRed) && keystate[SDL_SCANCODE_RIGHT]);

    // if both up and down are pressed, neither should be true
    if (Car::isUp && Car::isDown) {
        Car::isUp = false;
        Car::isDown = false;
    }

    // similar for left/right
    if (Car::isLeft && Car::isRight) {
        Car::isLeft = false;
        Car::isRight = false;
    }
}