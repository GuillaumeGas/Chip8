#include "Chip8.hpp"

using namespace std;

void Chip8::start () {
    _sc.update ();
    _handle_events ();
}

void Chip8::_handle_events () {
    bool stop = false;
    
    do {
	SDL_WaitEvent (&_event);

	switch (_event.type) {
	case SDL_QUIT:
	    stop = true;
	    break;
	case SDL_KEYDOWN:
	    break;
	default: break;
	}
    } while (!stop);
}
