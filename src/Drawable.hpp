#pragma once

#include <SDL2/SDL.h>

class Drawable
{
public:
	virtual void draw(SDL_Renderer * renderer) = 0;
	virtual void clear(SDL_Renderer * renderer) {}
};