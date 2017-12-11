#include "PauseController.hpp"
#include "Chip8.hpp"

PauseController::PauseController(Chip8 * mainController) 
	: BaseController<PauseScreen>(mainController)
{
	_screen = new PauseScreen(_mainController->getScreen()->getRenderer());
	_screen->setVisible(false);
	_screen->setRomPath(mainController->getRomFilePath());

	_screenType = PauseScreen::ScreenType::BROWSE_SCREEN;

	_browserController = new BrowserController(_mainController, _screen);
	_optionController = new OptionController(_mainController, _screen);
}

PauseController::~PauseController()
{
	if (_browserController != nullptr)
		delete _browserController;
	if (_optionController != nullptr)
		delete _optionController;
	if (_screen != nullptr)
		delete _screen;
}

void PauseController::handleKeyboard(Uint32 eventType, SDL_Keycode keyCode)
{
	switch (keyCode)
	{
	case SDLK_p:
		_screen->setVisible(!_screen->isVisible());
		break;
	case SDLK_F1:
		_screen->setScreenType(PauseScreen::ScreenType::BROWSE_SCREEN);
		_screenType = PauseScreen::ScreenType::BROWSE_SCREEN;
		break;
	case SDLK_F2:
		_screen->setScreenType(PauseScreen::ScreenType::OPTION_SCREEN);
		_screenType = PauseScreen::ScreenType::OPTION_SCREEN;
		break;
	default:
		if (_screenType == PauseScreen::ScreenType::BROWSE_SCREEN)
		{
			_browserController->handleKeyboard(eventType, keyCode);
		}
		else
		{
			_optionController->handleKeyboard(eventType, keyCode);
		}
	}
}

void PauseController::setRomePath(std::string newPath)
{
    _browserController->setRomPath(newPath);
}