#pragma once

#include "BaseController.hpp"

class Chip8;
class PauseScreen;

class BrowserController : public BaseController<PauseScreen>
{
public:
	BrowserController(Chip8 * mainController, PauseScreen * screen);

	void handleKeyboard(Uint32 eventType, SDL_Keycode keyCode) override;
};