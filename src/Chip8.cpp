#include "Chip8.hpp"

using namespace std;

Chip8::Chip8 (const char * file_name, bool debug) {
    this->sc = new Screen ();
    this->cpu = new Cpu (this->sc, debug);
    this->cpu->loadProgram (file_name);
}

Chip8::~Chip8 () {
    if (this->sc) delete this->sc;
    if (this->cpu) delete this->cpu;
}

void Chip8::start () {
    cout << "> Chip8 starting..." << endl;
    // this->sc.update ();
    thread_events = new thread (&Chip8::handle_events, this);
    this->cpu->start ();
}

void Chip8::handle_events () {
    cout << "> Thread events started." << endl;
    bool stop = false;

    do {
	SDL_WaitEvent (&_event);

	switch (_event.type) {
	case SDL_QUIT:
	    stop = true;
	    break;
	case SDL_KEYUP:
	    // switch (_event.key.keysym.sym) {
	    // case SDLK_ESCAPE:
	    // 	break;
	    // }
	    cout << "> Stopping Chip8..." << endl;
	    this->cpu->shutdown ();
	    stop = true;
	    break;
	default: break;
	}
    } while (!stop);
}
