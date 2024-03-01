#include "road.h"
#include <iostream>
#include "LEAGUE/engine.h"

Road::Road() {
    loadImage("./assets/1024x768slate.png");
}

void Road::draw(SDL_Renderer* renderer) {
/*     rect.w = 768;
    rect.h = 1024; */
    // std::cout << dest.x << ", " << dest.y << std::endl;
    int result = SDL_RenderCopyEx(renderer, texture, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    if (result != 0) {
        std::cout << SDL_GetError() << std::endl;
    }
}