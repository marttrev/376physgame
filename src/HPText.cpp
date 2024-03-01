#include "HPText.h"

#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <SDL2/SDL_ttf.h>

#include "LEAGUE/common_data_structures.h"
#include "user_data.h"

HPText::HPText(Car* car, SDL_Color textColor, int x, int y) {
    HPText::car = car;
    HPText::textColor = textColor;
    HPText::font = TTF_OpenFont("arial.ttf", 12);
    HPText::x = x;
    HPText::x = y;
}

HPText::~HPText(){
    TTF_CloseFont(HPText::font);
}

void HPText::draw(SDL_Renderer* renderer){
    SDL_Surface* surface;
    surface = TTF_RenderText_Solid(HPText::font, "" + HPText::car->userData.hp, HPText::textColor);
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    int text_width = surface->w;
    int text_height = surface->h;
    
    SDL_FreeSurface(surface);
    

    SDL_Rect dest;
    dest.x = HPText::x;
    dest.y = HPText::y;
    dest.w = text_width;
    dest.h = text_height;
    // std::cout << dest.x << ", " << dest.y << std::endl;
    int result = SDL_RenderCopyEx(renderer, texture, NULL, &dest, 0.0f, NULL, SDL_FLIP_NONE);
    if (result != 0) {
        std::cout << SDL_GetError() << std::endl;
    }
}