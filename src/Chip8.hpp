#pragma once

#include <iostream>
#include <cstdint>
#include <thread>
#include <SDL/SDL.h>

#include "Screen.hpp"
#include "Cpu.hpp"
#include "CpuDebug.hpp"
#include "Sound.hpp"

#define NB_KEYS 16

class Chip8 {
public:
    Chip8 (const char * file_name, bool debug);
    ~Chip8 ();

    void start ();

private:
    void loop_screen ();

    Screen * sc;
    Cpu * cpu;
    Sound sound;
    bool running;

    SDL_Event _event;

    const unsigned int keys[16] = { SDLK_KP7, SDLK_KP8, SDLK_KP9, SDLK_ASTERISK,
				    SDLK_KP4, SDLK_KP5, SDLK_KP6, SDLK_MINUS,
				    SDLK_KP1, SDLK_KP2, SDLK_KP3, SDLK_PLUS,
				    SDLK_RIGHT, SDLK_KP0, SDLK_PERIOD, SDLK_RETURN };
};
