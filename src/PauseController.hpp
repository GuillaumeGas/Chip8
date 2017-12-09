#pragma once

#include "PauseScreen.hpp"
#include "BaseController.hpp"
#include "BrowserController.hpp"
#include "OptionController.hpp"

class Chip8;

class PauseController : public BaseController<PauseScreen>
{
public:
	PauseController(Chip8 * mainController);
	~PauseController();

	void handleKeyboard(Uint32 eventType, SDL_Keycode keyCode) override;
	void setRomePath(std::string newPath);

private:
	PauseScreen::ScreenType _screenType;

	BrowserController * _browseController;
	OptionController * _optionController;
};