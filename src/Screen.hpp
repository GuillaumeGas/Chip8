#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

#include "Drawable.hpp"
#include "Exceptions.hpp"

#define SCREEN_WIDTH 64  // Pixels
#define SCREEN_HEIGHT 32 // Pixels

#define PIXEL_DIM 10   // real pixels

class Screen 
{
public:
	Screen();
	~Screen();

	void update();
	void addDrawable(Drawable * drawable);

	SDL_Renderer * getRenderer() const;

	enum ScreenType
	{
		GAME_SCREEN,
		PAUSE_SCREEN,
	};

private:
	void _initSdl();

	SDL_Window * _window;
	SDL_Renderer * _renderer;
	std::vector<Drawable*> _vecDrawables;
};
