#pragma once

#include <iostream>
#include <SDL/SDL.h>

#include "Exceptions.hpp"

#define SCREEN_WIDTH 64  // Pixels
#define SCREEN_HEIGHT 32 // Pixels
#define PIXEL_DIM 8   // real pixels

#define BLACK 0
#define WHITE 1

struct Pixel {
    SDL_Rect pos;
    Uint8 color;
};

class Screen {
public:
    Screen ();
    ~Screen ();

    void clear ();
    void update ();

private:
    void _init_sdl ();
    void _init_content ();
    void _draw_pixel (Pixel pixel);

    Pixel _content[SCREEN_WIDTH][SCREEN_HEIGHT];
    SDL_Surface * _screen_surf;
    SDL_Surface * _pixel_surf[2]; // pixel black and white
};
