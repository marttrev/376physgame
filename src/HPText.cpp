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
    HPText::x = x;
    HPText::y = y;
    HPText::font = TTF_OpenFont("./assets/OpenSans-Regular.ttf", 50);
    if(HPText::font == NULL){
        std::cout << "Could not load font: " << TTF_GetError() << std::endl;
    }
}

HPText::~HPText(){
    TTF_CloseFont(HPText::font);
}

// inspired by https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2/38169008#38169008
// however, it's been heavily modified
void HPText::draw(SDL_Renderer* renderer){
    const char* text = std::to_string(HPText::car->getUserData().hp).append("/5").c_str();

    SDL_Surface* surface;
    surface = TTF_RenderText_Solid(HPText::font, text, HPText::textColor);
    if(surface == NULL){
        std::cout << "Font not found: " << TTF_GetError() << std::endl;
        // return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture == NULL){
        std::cout << "error: " << SDL_GetError() << std::endl;
        // return;
    }
    SDL_FreeSurface(surface);
    
    SDL_Rect dest;
    dest.x = HPText::x;
    dest.y = HPText::y;
    TTF_SizeUTF8(HPText::font, text, &dest.w, &dest.h);

    int result = SDL_RenderCopy(renderer, texture, NULL, &dest);
    if (result != 0) {
        std::cout << SDL_GetError() << std::endl;
    }
}