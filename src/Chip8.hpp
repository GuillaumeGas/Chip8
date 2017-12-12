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

#define FPS 6
#define OP_PER_FRAME 1

class Chip8 {
public:
    Chip8 (const char * fileName, bool debug = false);
    ~Chip8 ();

    void start ();

	Screen * getScreen() const;
	Cpu * getCpu() const;
	Sound * getSound() const;
	std::string getRomFilePath() const;
	Uint32 getFps() const;
	void setFps(Uint32 value);
	Uint32 getOpPerFrame() const;
	void setOpPerFrame(Uint32 value);
	void loadRom(std::string filePath);

private:
	Screen * _screen;
	Cpu * _cpu;
	Sound * _sound;
	GameController * _gameController;
	PauseController * _pauseController;
	
	std::string _romFilePath;
	bool _running;
	bool _pause;
	Screen::ScreenType _screenType;
	Uint32 _fps;
	Uint32 _OpPerFrame;

    SDL_Event _event;
};
