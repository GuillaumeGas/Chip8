#include "Screen.hpp"

Screen::Screen()
{
	_initSdl();
}

Screen::~Screen()
{
	for (Drawable * elem : _vecDrawables)
		delete elem;

	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Screen::addDrawable(Drawable * drawable)
{
	_vecDrawables.push_back(drawable);
}

void Screen::_initSdl()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw SDLInitException(SDL_GetError());

	_window = SDL_CreateWindow("Chip-8 Emulator", 100, 100, SCREEN_WIDTH*PIXEL_DIM, SCREEN_HEIGHT*PIXEL_DIM, SDL_WINDOW_SHOWN);

	if (_window == nullptr)
		throw ScreenInitException(SDL_GetError());

	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == nullptr)
		throw PixelInitException(SDL_GetError());

	SDL_RenderSetLogicalSize(_renderer, SCREEN_WIDTH*PIXEL_DIM, SCREEN_HEIGHT*PIXEL_DIM);
}

//void Screen::_drawTest()
//{
//	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
//	SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 100);
//	SDL_Rect rect;
//	rect.h = rect.w = 300;
//	rect.x = rect.y = 10;
//	SDL_RenderFillRect(_renderer, &rect);
//}

void Screen::clear()
{
	for (Drawable * elem : _vecDrawables)
		elem->clear(_renderer);

	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);
}

void Screen::update()
{
	for (Drawable * elem : _vecDrawables)
		elem->draw(_renderer);

	SDL_RenderPresent(_renderer);
}
