#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Screen.hpp"
#include "Drawable.hpp"

#define PAUSE_SCREEN_OFFSET 50
#define HEADER_HEIGHT 30

#define PATH_MAX_LENGTH 50

class PauseScreen : public Drawable
{
public:
	PauseScreen(SDL_Renderer * renderer);
	~PauseScreen();

	void drawElement();
	void setRomPath(std::string newPath);

private:
	void _initText();
	void _buildTexture(std::string & str, SDL_Texture * texture);
	std::string _getShortPath(std::string & path);

	TTF_Font * _font;
	SDL_Surface * _surface;
	SDL_Texture * _textureDir;
	SDL_Rect _rectDir;

	const SDL_Rect _backgroudRect
	{
		PAUSE_SCREEN_OFFSET,
		PAUSE_SCREEN_OFFSET / 2,
		(SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
		(SCREEN_HEIGHT * PIXEL_DIM) - PAUSE_SCREEN_OFFSET,
	};

	const SDL_Rect _headerRect
	{
		PAUSE_SCREEN_OFFSET,
		PAUSE_SCREEN_OFFSET / 2,
		(SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
		HEADER_HEIGHT,
	};

	const SDL_Rect _footerRect
	{
		PAUSE_SCREEN_OFFSET,
		(SCREEN_HEIGHT * PIXEL_DIM) - ((PAUSE_SCREEN_OFFSET / 2) + HEADER_HEIGHT),
		(SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
		HEADER_HEIGHT,
	};

	const SDL_Color _backgroundColor { 0, 102, 0, 150 };
	const SDL_Color _borderColor { 0, 153, 0, 255 };
	const SDL_Color _headerColor{ 2, 73, 2, 170 };
};