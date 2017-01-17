#pragma once

#include <iostream>
#include <SDL/SDL.h>

#include "Screen.hpp"
#include "Cpu.hpp"

class Chip8 {
public:
    Chip8 ();
    ~Chip8 ();

    void start ();

private:
    void _handle_events ();

    Screen * _sc;
    Cpu * _cpu;

    SDL_Event _event;
};
