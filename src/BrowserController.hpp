#pragma once

#include "BaseController.hpp"
#include "Utils/Directory.hpp"

class Chip8;
class PauseScreen;

class BrowserController : public BaseController<PauseScreen>
{
public:
	BrowserController(Chip8 * mainController, PauseScreen * screen);

	void handleKeyboard(Uint32 eventType, SDL_Keycode keyCode) override;
    void setRomPath(std::string newPath);

private:
    Directory _romDir;
};