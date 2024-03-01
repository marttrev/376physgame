#ifndef		__H__SCORE_TEXT__
#define		__H__SCORE_TEXT__

#include <SDL2/SDL_ttf.h>

#include "LEAGUE/common_data_structures.h"
#include "car.h"

class HPText : public Drawable {
	public:
		HPText(Car*, SDL_Color, int x, int y);
		~HPText();
		void draw(SDL_Renderer* renderer) override;
	private:
		Car* car;
		SDL_Color textColor;
		TTF_Font* font;
		int x;
		int y;
};

#endif
