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
	_pause = false;
	_screenType = Screen::ScreenType::GAME_SCREEN;
	_fps = FPS;
	_OpPerFrame = OP_PER_FRAME;
}

void Chip8::loadRom(string filePath)
{
	_gameController->reset();
	_pauseController->setRomePath(filePath);
	_cpu->loadProgram(filePath.c_str());
	_pause = false;
	_screenType = Screen::ScreenType::GAME_SCREEN;
}

Chip8::~Chip8()
{
	if (_gameController)
		delete _gameController;
	if (_pauseController)
		delete _pauseController;
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

	bool stop = false;
	while (!stop && _cpu->isRunning())
	{
		if (!_pause)
		{
			for(Uint32 i = 0; i < _OpPerFrame; i++)
				stop = !_cpu->emulateCycle();
		}

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
					if (_screenType == Screen::ScreenType::GAME_SCREEN)
					{
						if (_event.key.keysym.sym == SDLK_p)
						{
							_pauseController->handleKeyboard(_event.type, _event.key.keysym.sym);
							_screenType = Screen::ScreenType::PAUSE_SCREEN;
							_pause = true;
						}
						else 
						{
							_gameController->handleKeyboard(_event.type, _event.key.keysym.sym);
						}
					}
					else
					{
						_pauseController->handleKeyboard(_event.type, _event.key.keysym.sym);
						if (_event.key.keysym.sym == SDLK_p)
						{
							_screenType = Screen::ScreenType::GAME_SCREEN;
							_pause = false;
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

		SDL_Delay(_fps);
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

Uint32 Chip8::getFps() const
{
	return _fps;
}

void Chip8::setFps(Uint32 value)
{
	_fps = value;
}

Uint32 Chip8::getOpPerFrame() const
{
	return _OpPerFrame;
}

void Chip8::setOpPerFrame(Uint32 value)
{
	_OpPerFrame = value;
}