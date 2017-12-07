#include "PauseController.hpp"
#include "Chip8.hpp"

PauseController::PauseController(Chip8 * mainController) : _mainController(mainController)
{
	_screen = new PauseScreen(_mainController->getScreen()->getRenderer());
	_screen->setVisible(false);
	_screen->setRomPath(mainController->getRomFilePath());
}

PauseController::~PauseController()
{
	if (_screen == nullptr)
		delete _screen;
}

PauseScreen * PauseController::getScreen() const
{
	return _screen;
}

void PauseController::handleKeyboard(Uint32 eventType, SDL_Keycode keyCode)
{
	if (keyCode == SDLK_p)
		_screen->setVisible(!_screen->isVisible());
}

void PauseController::setRomePath(std::string newPath)
{
	_screen->setRomPath(newPath);
}