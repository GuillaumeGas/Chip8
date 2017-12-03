#include "Chip8.hpp"
#include "GameScreen.hpp"

using namespace std;

Chip8::Chip8(const char * file_name, bool debug) 
{
	screen = new Screen();
	gameScreen = new GameScreen();
	sound = new Sound();

	screen->addDrawable(gameScreen);

	cpu = debug ? new CpuDebug(this) : new Cpu(this);
	cpu->loadProgram(file_name);
	_running = true;
}

Chip8::~Chip8() 
{
	// We don't need to free the gameScreen because it is freed in the Screen class
	if (screen)
		delete screen;
	if (cpu) 
		delete cpu;
	if (sound)
		delete sound;
}

void Chip8::start() 
{
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
					if (_keys[i] == _event.key.keysym.sym) {
						cpu->keyboard[i] = 1;
					}
			}
			else if (_event.type == SDL_KEYUP) {
				if (_event.key.keysym.sym == SDLK_ESCAPE) {
					stop = true;
				}
				else {
					for (int i = 0; i < NB_KEYS; i++)
						if (_keys[i] == _event.key.keysym.sym)
							cpu->keyboard[i] = 0;
				}
			}
		}

		screen->update();

		if (cpu->sound_timer) {
			sound->play();
			cpu->sound_timer = 0;
		}

		SDL_Delay(FPS);
	}

	_running = false;
}