#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

#include "Drawable.hpp"
#include "Exceptions.hpp"

#define SCREEN_WIDTH 64  // Pixels
#define SCREEN_HEIGHT 32 // Pixels

#define PIXEL_DIM 10   // real pixels

#define BLACK 0
#define WHITE 255

class Screen {
public:
	Screen();
	~Screen();

	void clear();
	void update();

	void addDrawable(Drawable * drawable);

private:
	void _initSdl();
	void _initContent();
	//void _drawTest();

	SDL_Window * _window;
	SDL_Renderer * _renderer;
	std::vector<Drawable*> _vecDrawables;
};
