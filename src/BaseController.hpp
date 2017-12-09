#pragma once

#include "SDL2\SDL.h"

class Chip8;

template<class ScreenType>
class BaseController 
{
public:
	ScreenType * getScreen() const { return _screen; }
	virtual void handleKeyboard(Uint32 eventType, SDL_Keycode keyCode) = 0;

protected:
	BaseController(Chip8 * mainController) : _mainController(mainController) {}
	BaseController(Chip8 * mainController, ScreenType * screen) : _mainController(mainController), _screen(screen) {}

	ScreenType * _screen;
	Chip8 * _mainController;
};