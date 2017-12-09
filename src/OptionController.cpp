#include "OptionController.hpp"
#include "Chip8.hpp"
#include "PauseScreen.hpp"

using namespace std;

OptionController::OptionController(Chip8 * mainController, PauseScreen * screen)
	: BaseController<PauseScreen>(mainController, screen) {}

void OptionController::handleKeyboard(Uint32 eventType, SDL_Keycode keyCode)
{
	Uint32 fps = _mainController->getFps();
	Uint32 opPerFrame = _mainController->getOpPerFrame();
	if (keyCode == SDLK_KP_PLUS)
	{
		_mainController->setFps(++fps);
	}
	else if (keyCode == SDLK_KP_MINUS)
	{
		if (fps > 0)
			_mainController->setFps(--fps);
	}
	else if (keyCode == SDLK_a)
	{
		_mainController->setOpPerFrame(++opPerFrame);
	}
	else if (keyCode == SDLK_z)
	{
		if (opPerFrame > 0)
			_mainController->setOpPerFrame(--opPerFrame);
	}
}