#include "BrowserController.hpp"
#include "Chip8.hpp"
#include "PauseScreen.hpp"

using namespace std;

BrowserController::BrowserController(Chip8 * mainController, PauseScreen * screen) 
	: BaseController<PauseScreen>(mainController, screen) {}

void BrowserController::handleKeyboard(Uint32 eventType, SDL_Keycode keyCode)
{

}

void BrowserController::setRomPath(string newPath)
{
    _romDir.setCurrent(Directory::RemoveFileName(newPath));

    auto vec = _romDir.getFiles();
    auto subVecStart = vec.begin();
    auto subVecEnd = subVecStart + 10;
    _screen->setFilesList(vector<File>(subVecStart, subVecEnd));
    _screen->setRomPath(newPath);
}