#ifndef		__H__ROAD__
#define		__H__ROAD__

#include "LEAGUE/engine.h"
#include <iostream>

class Road : public Drawable {//only drawable because it is a static body
	public:
		Road();
		void draw(SDL_Renderer* renderer) override;
};

#endif

