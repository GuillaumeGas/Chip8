#include "BrowserController.hpp"
#include "Chip8.hpp"
#include "PauseScreen.hpp"

using namespace std;

BrowserController::BrowserController(Chip8 * mainController, PauseScreen * screen)
	: BaseController<PauseScreen>(mainController, screen) {}

void BrowserController::handleKeyboard(Uint32 eventType, SDL_Keycode keyCode)
{
	switch (keyCode)
	{
	case SDLK_UP:
		if (_fileIndex > 0)
		{
			_fileIndex--;
			if (_screenFileLineIndex > 0)
				_screen->setSelectedLineIndex(--_screenFileLineIndex);
			else
				_updateScreen();
		}
		break;
	case SDLK_DOWN:
		if (_fileIndex < _romDir.getFiles().size())
		{
			_fileIndex++;
			if (_screenFileLineIndex < 9)
				_screen->setSelectedLineIndex(++_screenFileLineIndex);
			else
				_updateScreen();
		}
		break;
	case SDLK_RETURN:
		File file = _romDir.getFiles()[_fileIndex];
		string filePath = file.fileFullPath;

		if (filePath.size() >= 2 && filePath.substr(filePath.size() - 2) == ".."
			|| file.fileType == FileType::DIRECTORY_TYPE)
		{
			_romDir.setCurrent(filePath);
			_fileIndex = 0;
			_screenFileLineIndex = 0;
			_screen->setSelectedLineIndex(0);
			_updateScreen();
		}
		else
		{
			_mainController->loadRom(filePath);
			_screen->setVisible(!_screen->isVisible());
		}
		break;
	}
}

void BrowserController::setRomPath(string newPath)
{
	_romDir.setCurrent(Directory::RemoveFileName(newPath));
	_fileIndex = 0;
	_screenFileLineIndex = 0;
	_updateScreen(newPath);
}

void BrowserController::_updateScreen(string newPath)
{
	if (newPath.length() > 0)
	{
		_screen->setRomPath(newPath);
		_screen->setSelectedLineIndex(0);
	}

	auto vec = _romDir.getFiles();

	size_t offsetStart = (_fileIndex > 9 ? (_fileIndex - 9) : _fileIndex);
	size_t offsetEnd = 10;
	if ((offsetStart + offsetEnd) >= vec.size())
		offsetEnd -= ((offsetStart + offsetEnd) - vec.size());

	auto subVecStart = vec.begin() + offsetStart;
	auto subVecEnd = subVecStart + offsetEnd;
	_screen->setFilesList(vector<File>(subVecStart, subVecEnd));
}