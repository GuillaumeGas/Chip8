#include "GameScreen.hpp"

GameScreen::GameScreen()
{
	_initContent();
}

void GameScreen::drawElement(SDL_Renderer * renderer)
{
	for (int i = 0; i < SCREEN_WIDTH; i++) 
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++) 
		{
			_drawPixel(renderer, _content[i][j]);
		}
	}
}

void GameScreen::clearElement(SDL_Renderer * renderer)
{
	for (int i = 0; i < SCREEN_WIDTH; i++) 
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++) 
		{
			_content[i][j].color = BLACK;
		}
	}
}

Pixel GameScreen::getPixel(int x, int y)
{
	return _content[x][y];
}

void GameScreen::setColor(int x, int y, int color) 
{
	_content[x][y].color = color;
}

void GameScreen::_drawPixel(SDL_Renderer * renderer, Pixel pixel) 
{
	SDL_SetRenderDrawColor(renderer, pixel.color, pixel.color, pixel.color, 255);
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
			_content[i][j].color = BLACK;
		}
	}
}