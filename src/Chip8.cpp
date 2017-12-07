#include "Chip8.hpp"
#include "Utils\Directory.hpp"

using namespace std;

Chip8::Chip8(const char * fileName, bool debug)
{
	_screen = new Screen();
	_sound = new Sound();

	_gameController = new GameController(this);
	_pauseController = new PauseController(this);
	_pauseController->setRomePath(Directory::GetFullFilePath(string(fileName)));

	_screen->addDrawable(static_cast<Drawable*>(_gameController->getScreen()));
	_screen->addDrawable(static_cast<Drawable*>(_pauseController->getScreen()));

	_cpu = debug ? new CpuDebug(_gameController->getScreen()) : new Cpu(_gameController->getScreen());
	_cpu->loadProgram(fileName);
	_running = true;
}

Chip8::~Chip8()
{
	// We don't need to free the gameScreen because it is freed in the Screen class
	if (_screen)
		delete _screen;
	if (_cpu)
		delete _cpu;
	if (_sound)
		delete _sound;
}

void Chip8::start()
{
	cout << "> Chip8 starting..." << endl;

	bool stop = false, pause = false;
	while (!stop && _cpu->isRunning())
	{
		if (!pause)
			_cpu->emulateCycle();

		while (SDL_PollEvent(&_event))
		{
			if (_event.type == SDL_QUIT)
			{
				stop = true;
			}
			else if (_event.type == SDL_KEYDOWN)
			{
				_gameController->handleKeyboard(_event.type, _event.key.keysym.sym);
			}
			else if (_event.type == SDL_KEYUP)
			{
				if (_event.key.keysym.sym == SDLK_ESCAPE)
				{
					stop = true;
				}
				else
				{
					if (_screenType == ScreenType::GAME_SCREEN)
					{
						if (_event.key.keysym.sym == SDLK_p)
						{
							_pauseController->handleKeyboard(_event.type, _event.key.keysym.sym);
							_screenType = ScreenType::PAUSE_SCREEN;
							pause = true;
						}
						else 
						{
							_gameController->handleKeyboard(_event.type, _event.key.keysym.sym);
						}
					}
					else
					{
						if (_event.key.keysym.sym == SDLK_p)
						{
							_pauseController->handleKeyboard(_event.type, _event.key.keysym.sym);
							_screenType = ScreenType::GAME_SCREEN;
							pause = false;
						}
					}
				}
			}
		}

		_screen->update();

		if (_cpu->sound_timer) 
		{
			_sound->play();
			_cpu->sound_timer = 0;
		}

		SDL_Delay(FPS);
	}

	_running = false;
}

Screen * Chip8::getScreen() const
{
	return _screen;
}

Cpu * Chip8::getCpu() const
{
	return _cpu;
}

Sound * Chip8::getSound() const
{
	return _sound;
}

string Chip8::getRomFilePath() const
{
	return _romFilePath;
}