#include "Screen.hpp"

Screen::Screen () {
    _init_sdl ();
    _init_content ();
}

Screen::~Screen () {
    SDL_FreeSurface (_pixel_surf[0]);
    SDL_FreeSurface (_pixel_surf[1]);
    SDL_Quit ();
}

void Screen::_init_sdl () {
    if (SDL_Init (SDL_INIT_VIDEO) != 0)
    	throw SDLInitException (SDL_GetError ());

    _screen_surf = NULL;
    _pixel_surf[0] = NULL;
    _pixel_surf[1] = NULL;

    _screen_surf = SDL_SetVideoMode (SCREEN_WIDTH*PIXEL_DIM, SCREEN_HEIGHT*PIXEL_DIM, 32, SDL_HWSURFACE);
    SDL_WM_SetCaption ("Chip-8 Emulator", NULL);

    if (_screen_surf == NULL)
    	throw ScreenInitException (SDL_GetError ());

    _pixel_surf[0] = SDL_CreateRGBSurface (SDL_HWSURFACE, PIXEL_DIM, PIXEL_DIM, 32, 0, 0, 0, 0);
    if (_pixel_surf[0] == NULL)
    	throw PixelInitException (SDL_GetError ());

    SDL_FillRect (_pixel_surf[0], NULL, SDL_MapRGB (_pixel_surf[0]->format, 0x00, 0x00, 0x00));

    _pixel_surf[1] = SDL_CreateRGBSurface (SDL_HWSURFACE, PIXEL_DIM, PIXEL_DIM, 32, 0, 0, 0, 0);
    if (_pixel_surf[1] == NULL)
    	throw PixelInitException (SDL_GetError ());

    SDL_FillRect (_pixel_surf[1], NULL, SDL_MapRGB (_pixel_surf[1]->format, 0xFF, 0xFF, 0xFF));
}

void Screen::_draw_pixel (Pixel pixel) {
    SDL_BlitSurface (_pixel_surf[pixel.color], NULL, _screen_surf, &pixel.pos);
}

void Screen::clear () {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
	for (int j = 0; j < SCREEN_HEIGHT; j++) {
	    _content[i][j].color = BLACK;
	}
    }

    SDL_FillRect (_screen_surf, NULL, BLACK);
}

void Screen::update () {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
	for (int j = 0; j < SCREEN_HEIGHT; j++) {
	    _draw_pixel (_content[i][j]);
	}
    }

    SDL_Flip (_screen_surf);
}

void Screen::_init_content () {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
	for (int j = 0; j < SCREEN_HEIGHT; j++) {
	    _content[i][j].pos.x = i * PIXEL_DIM;
	    _content[i][j].pos.y = j * PIXEL_DIM;
	    _content[i][j].color = BLACK;
	}
    }
}
