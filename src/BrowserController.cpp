#include "BrowserController.hpp"
#include "Chip8.hpp"
#include "PauseScreen.hpp"

BrowserController::BrowserController(Chip8 * mainController, PauseScreen * screen) 
	: BaseController<PauseScreen>(mainController, screen) {}

void BrowserController::handleKeyboard(Uint32 eventType, SDL_Keycode keyCode)
{

}