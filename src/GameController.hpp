#pragma once

#include "GameScreen.hpp"
#include "BaseController.hpp"

#define NB_KEYS 16

class Chip8;

class GameController : public BaseController<GameScreen>
{
public:
	GameController(Chip8 * mainController);
	~GameController();

	void handleKeyboard(Uint32 eventType, SDL_Keycode keyCode) override;
	void reset();

private:
	const unsigned int _keys[16] = { SDLK_KP_7, SDLK_KP_8, SDLK_KP_9, SDLK_ASTERISK,
		SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_MINUS,
		SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_PLUS,
		SDLK_RIGHT, SDLK_KP_0, SDLK_KP_PERIOD, SDLK_KP_ENTER };
};