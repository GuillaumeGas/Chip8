#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include "Exceptions.hpp"

#define SCREEN_WIDTH 64  // Pixels
#define SCREEN_HEIGHT 32 // Pixels
#define PIXEL_DIM 10   // real pixels

#define BLACK 0
#define WHITE 255

struct Pixel {
	SDL_Rect pos;
	Uint8 color;
};

class Screen {
public:
	Screen();
	~Screen();

	void clear();
	void update();

	Pixel getPixel(int x, int y);
	void setColor(int x, int y, int color);

private:
	void _init_sdl();
	void _init_content();
	void _draw_pixel(Pixel pixel);

	Pixel _content[SCREEN_WIDTH][SCREEN_HEIGHT];
	SDL_Window * _window;
	SDL_Renderer * _renderer;
};
