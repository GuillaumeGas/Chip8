#pragma once

#include <SDL2/SDL.h>

#include "Screen.hpp"
#include "Drawable.hpp"

struct Pixel 
{
	SDL_Rect pos;
	bool isEnabled;
};

class GameScreen : public Drawable
{
public:
	GameScreen(SDL_Renderer * renderer);

	void drawElement();
	void clear();

	Pixel & getPixel(int x, int y);

	static const SDL_Color EnabledPixelColor;
	static const SDL_Color DisabledPixelColor;

private:
	void _initContent();
	void _drawPixel(SDL_Renderer * renderer, Pixel pixel);

	Pixel _content[SCREEN_WIDTH][SCREEN_HEIGHT];
};