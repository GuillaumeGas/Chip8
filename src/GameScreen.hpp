#pragma once

#include <SDL2/SDL.h>

#include "Screen.hpp"
#include "Drawable.hpp"

struct Pixel 
{
	SDL_Rect pos;
	Uint8 color;
};

class GameScreen : public Drawable
{
public:
	GameScreen();

	void drawElement(SDL_Renderer * renderer);
	void clearElement(SDL_Renderer * renderer);

	Pixel getPixel(int x, int y);
	void setColor(int x, int y, int color);

private:
	void _initContent();
	void _drawPixel(SDL_Renderer * renderer, Pixel pixel);

	Pixel _content[SCREEN_WIDTH][SCREEN_HEIGHT];
};