#include "Screen.hpp"

Screen::Screen() {
	_init_sdl();
	_init_content();
}

Screen::~Screen() {
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Screen::_init_sdl() {
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

void Screen::_draw_pixel(Pixel pixel) {
	SDL_SetRenderDrawColor(_renderer, pixel.color, pixel.color, pixel.color, 255);
	SDL_RenderFillRect(_renderer, &pixel.pos);
}

void Screen::clear() {
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			_content[i][j].color = BLACK;
		}
	}

	SDL_RenderClear(_renderer);
	SDL_RenderPresent(_renderer);
}

void Screen::update() {
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			_draw_pixel(_content[i][j]);
		}
	}

	SDL_RenderPresent(_renderer);
}

Pixel Screen::getPixel(int x, int y) {
	return _content[x][y];
}

void Screen::setColor(int x, int y, int color) {
	_content[x][y].color = color;
}

void Screen::_init_content() {
	for (int i = 0; i < SCREEN_WIDTH; i++) {
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			_content[i][j].pos.x = i * PIXEL_DIM;
			_content[i][j].pos.y = j * PIXEL_DIM;
			_content[i][j].pos.w = PIXEL_DIM;
			_content[i][j].pos.h = PIXEL_DIM;
			_content[i][j].color = BLACK;
		}
	}
}
