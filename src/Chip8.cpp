#include "Chip8.hpp"

using namespace std;

Chip8::Chip8 () {
    _sc = new Screen ();
    _cpu = new Cpu (_sc);
}

Chip8::~Chip8 () {
    if (_sc) delete _sc;
    if (_cpu) delete _cpu;
}

void Chip8::start () {
    // _sc.update ();
    // _handle_events ();
    _cpu->start ();
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
