#include "Screen.hpp"

Screen::Screen()
{
	_initSdl();
}

Screen::~Screen()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Screen::addDrawable(Drawable * drawable)
{
	_vecDrawables.push_back(drawable);
}

void Screen::update()
{
	for (Drawable * elem : _vecDrawables)
		elem->draw();

	SDL_RenderPresent(_renderer);
}

SDL_Renderer * Screen::getRenderer() const
{
	return _renderer;
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
