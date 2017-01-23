#pragma once

#include <iostream>
#include <thread>
#include <SDL/SDL.h>

#include "Screen.hpp"
#include "Cpu.hpp"

class Chip8 {
public:
    Chip8 (const char * file_name, bool debug);
    ~Chip8 ();

    void start ();

private:
    void handle_events ();

    Screen * sc;
    Cpu * cpu;

    SDL_Event _event;
    std::thread * thread_events;
};
