#pragma once

#include "BaseController.hpp"

class Chip8;
class PauseScreen;

class OptionController : public BaseController<PauseScreen>
{
public:
	OptionController(Chip8 * mainController, PauseScreen * screen);

	void handleKeyboard(Uint32 eventType, SDL_Keycode keyCode) override;
};