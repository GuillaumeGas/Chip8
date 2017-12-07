#pragma once

#include "PauseScreen.hpp"

class Chip8;

class PauseController
{
public:
	PauseController(Chip8 * mainController);
	~PauseController();

	PauseScreen * getScreen() const;
	void handleKeyboard(Uint32 eventType, SDL_Keycode keyCode);	
	void setRomePath(std::string newPath);

private:
	PauseScreen * _screen;
	Chip8 * _mainController;
};