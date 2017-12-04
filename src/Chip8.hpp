#pragma once

#include <iostream>
#include <cstdint>
#include <SDL2/SDL.h>

#include "Screen.hpp"
#include "GameScreen.hpp"
#include "PauseScreen.hpp"
#include "Cpu.hpp"
#include "CpuDebug.hpp"
#include "Sound.hpp"

#define NB_KEYS 16

enum ScreenType
{
	GAME_SCREEN,
	PAUSE_SCREEN,
};

class Chip8 {
public:
    Chip8 (const char * file_name, bool debug = false);
    ~Chip8 ();

    void start ();

	Screen * screen;
	GameScreen * gameScreen;
	PauseScreen * pauseScreen;
	Cpu * cpu;
	Sound * sound;

private:
    bool _running;
	ScreenType _screenType;

    SDL_Event _event;

    const unsigned int _keys[16] = { SDLK_KP_7, SDLK_KP_8, SDLK_KP_9, SDLK_ASTERISK,
				    SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_MINUS,
				    SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_PLUS,
				    SDLK_RIGHT, SDLK_KP_0, SDLK_KP_PERIOD, SDLK_KP_ENTER };
};
