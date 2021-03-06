#include "Chip8.hpp"

using namespace std;

Chip8::Chip8(const char * file_name, bool debug) {
	this->sc = new Screen();
	this->cpu = debug ? new CpuDebug(this, this->sc) : new Cpu(this, this->sc);
	this->cpu->loadProgram(file_name);
	this->running = true;
}

Chip8::~Chip8() {
	if (this->sc) delete this->sc;
	if (this->cpu) delete this->cpu;
}

void Chip8::start() {
	cout << "> Chip8 starting..." << endl;

	bool stop = false;
	while (!stop && cpu->isRunning()) {
		cpu->emulateCycle();

		while (SDL_PollEvent(&_event)) {
			if (_event.type == SDL_QUIT) {
				stop = true;
			}
			else if (_event.type == SDL_KEYDOWN) {
				for (int i = 0; i < NB_KEYS; i++)
					if (keys[i] == _event.key.keysym.sym) {
						this->cpu->keyboard[i] = 1;
					}
			}
			else if (_event.type == SDL_KEYUP) {
				if (_event.key.keysym.sym == SDLK_ESCAPE) {
					stop = true;
				}
				else {
					for (int i = 0; i < NB_KEYS; i++)
						if (keys[i] == _event.key.keysym.sym)
							this->cpu->keyboard[i] = 0;
				}
			}
		}

		this->sc->update();

		if (this->cpu->sound_timer) {
			sound.play();
			this->cpu->sound_timer = 0;
		}

		SDL_Delay(FPS);
	}

	this->running = false;
}