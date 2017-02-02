#pragma once

#include <iostream>
#include <SDL/SDL_mixer.h>

#include "Exceptions.hpp"

class Sound {
public:
    Sound ();
    ~Sound ();

    void play ();

private:
    Mix_Chunk * sound;
};
