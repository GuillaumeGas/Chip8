#include "GameScreen.hpp"

const SDL_Color GameScreen::EnabledPixelColor = { 51, 102, 0 };
const SDL_Color GameScreen::DisabledPixelColor = { 0, 0, 0 };

GameScreen::GameScreen(SDL_Renderer * renderer) : Drawable(renderer)
{
	_initContent();
}

void GameScreen::drawElement()
{
	for (int i = 0; i < SCREEN_WIDTH; i++) 
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++) 
		{
			_drawPixel(_renderer, _content[i][j]);
		}
	}
}

void GameScreen::clear()
{
	for (int i = 0; i < SCREEN_WIDTH; i++) 
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++) 
		{
			_content[i][j].isEnabled = false;
		}
	}
}

Pixel & GameScreen::getPixel(int x, int y)
{
	return _content[x][y];
}

void GameScreen::_drawPixel(SDL_Renderer * renderer, Pixel pixel) 
{
	const SDL_Color & color = (pixel.isEnabled) ? EnabledPixelColor : DisabledPixelColor;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &pixel.pos);
}

void GameScreen::_initContent() 
{
	for (int i = 0; i < SCREEN_WIDTH; i++) 
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++) 
		{
			_content[i][j].pos.x = i * PIXEL_DIM;
			_content[i][j].pos.y = j * PIXEL_DIM;
			_content[i][j].pos.w = PIXEL_DIM;
			_content[i][j].pos.h = PIXEL_DIM;
			_content[i][j].isEnabled = false;
		}
	}
}