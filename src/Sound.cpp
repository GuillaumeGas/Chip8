#include "Sound.hpp"

using namespace std;

Sound::Sound () {
    if (Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	throw SoundErrorException (Mix_GetError ());

    this->sound = Mix_LoadWAV ("sound.wav");
}

Sound::~Sound () {
    if (this->sound)
	Mix_FreeChunk (this->sound);
    Mix_CloseAudio ();
}

void Sound::play () {
    Mix_PlayChannel (0, this->sound, 0);
}
