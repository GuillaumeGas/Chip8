#include "Chip8.hpp"

using namespace std;

Chip8::Chip8(const char * file_name, bool debug)
{
	screen = new Screen();
	gameScreen = new GameScreen();
	pauseScreen = new PauseScreen();
	pauseScreen->setVisible(false);

	sound = new Sound();

	screen->addDrawable(gameScreen);
	screen->addDrawable(pauseScreen);

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

	bool stop = false, pause = false;
	while (!stop && cpu->isRunning())
	{
		if (!pause)
			cpu->emulateCycle();

		while (SDL_PollEvent(&_event))
		{
			if (_event.type == SDL_QUIT)
			{
				stop = true;
			}
			else if (_event.type == SDL_KEYDOWN)
			{
				for (int i = 0; i < NB_KEYS; i++)
					if (_keys[i] == _event.key.keysym.sym)
						cpu->keyboard[i] = 1;
			}
			else if (_event.type == SDL_KEYUP)
			{
				if (_screenType == ScreenType::GAME_SCREEN)
				{
					if (_event.key.keysym.sym == SDLK_ESCAPE)
					{
						stop = true;
					}
					else if (_event.key.keysym.sym == SDLK_p)
					{
						pauseScreen->setVisible(true);
						_screenType = ScreenType::PAUSE_SCREEN;
						pause = true;
					}
					else {
						for (int i = 0; i < NB_KEYS; i++)
							if (_keys[i] == _event.key.keysym.sym)
								cpu->keyboard[i] = 0;
					}
				}
				else
				{
					if (_event.key.keysym.sym == SDLK_p)
					{
						pauseScreen->setVisible(false);
						_screenType = ScreenType::GAME_SCREEN;
						pause = false;
					}
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