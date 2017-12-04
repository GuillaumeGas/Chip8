#pragma once

#include <SDL2/SDL.h>

#include "Screen.hpp"
#include "Drawable.hpp"

#define PAUSE_SCREEN_OFFSET 50

class PauseScreen : public Drawable
{
public:
	void drawElement(SDL_Renderer * renderer);

private:
	const SDL_Rect _rect
	{
		PAUSE_SCREEN_OFFSET,
		PAUSE_SCREEN_OFFSET,
		(SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
		(SCREEN_HEIGHT * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
	};
};