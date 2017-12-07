#include "GameController.hpp"
#include "Chip8.hpp"

GameController::GameController(Chip8 * mainController) : _mainController(mainController) 
{
	_screen = new GameScreen(mainController->getScreen()->getRenderer());
}

GameController::~GameController()
{
	if (_screen != nullptr)
		delete _screen;
}

GameScreen * GameController::getScreen() const
{
	return _screen;
}

void GameController::handleKeyboard(Uint32 eventType, SDL_Keycode keyCode)
{
	for (int i = 0; i < NB_KEYS; i++)
		if (_keys[i] == keyCode)
			_mainController->getCpu()->keyboard[i] = (eventType == SDL_EventType::SDL_KEYDOWN ? 1 : 0);
}