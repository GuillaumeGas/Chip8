#pragma once 

#include <iostream>
#include <cstdint>
#include <SDL2/SDL.h>

#include "Screen.hpp"
#include "GameController.hpp"
#include "PauseController.hpp"
#include "Cpu.hpp"
#include "CpuDebug.hpp"
#include "Sound.hpp"

enum ScreenType
{
	GAME_SCREEN,
	PAUSE_SCREEN,
};

class Chip8 {
public:
    Chip8 (const char * fileName, bool debug = false);
    ~Chip8 ();

    void start ();

	Screen * getScreen() const;
	Cpu * getCpu() const;
	Sound * getSound() const;
	std::string getRomFilePath() const;

private:
	Screen * _screen;
	Cpu * _cpu;
	Sound * _sound;
	GameController * _gameController;
	PauseController * _pauseController;
	
	std::string _romFilePath;
	bool _running;
	ScreenType _screenType;

    SDL_Event _event;
};
