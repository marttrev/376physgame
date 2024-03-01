#include "road.h"
#include <iostream>
#include "LEAGUE/engine.h"

Road::Road() {
    loadImage("./assets/1024x768slate.png");
}

void Road::draw(SDL_Renderer* renderer) {
    // Just draw to the whole screen since this is just a background.
    int result = SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    if (result != 0) {
        std::cout << SDL_GetError() << std::endl;
    }
}