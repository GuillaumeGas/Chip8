#pragma once

#include <iostream>
#include <SDL/SDL.h>

#include "Screen.hpp"
#include "Cpu.hpp"

class Chip8 {
public:
    void start ();

private:
    void _handle_events ();

    Screen _sc;
    Cpu _cpu;
    
    SDL_Event _event;
};
