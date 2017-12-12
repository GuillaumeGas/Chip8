#include "PauseScreen.hpp"
#include <string>

using namespace std;

PauseScreen::PauseScreen(SDL_Renderer * renderer) 
	: Drawable(renderer), _screenType(ScreenType::BROWSE_SCREEN), _font(nullptr), _textureDir(nullptr), _selectedLineIndex(0)
{
	TTF_Init();
	_initText();
}

PauseScreen::~PauseScreen()
{
	if (_textureDir != nullptr)
		SDL_DestroyTexture(_textureDir);
	TTF_CloseFont(_font);
	TTF_Quit();
}

void PauseScreen::setScreenType(ScreenType type)
{
	_screenType = type;
}

void PauseScreen::drawElement()
{
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

	_drawBackground();

	if (_screenType == ScreenType::BROWSE_SCREEN)
	{
		_drawBrowser();
	}
	else
	{
		_drawOption();
	}

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
}

void PauseScreen::_drawBackground()
{
	SDL_SetRenderDrawColor(_renderer, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	SDL_RenderFillRect(_renderer, &_backgroudRect);

	SDL_SetRenderDrawColor(_renderer, _headerColor.r, _headerColor.g, _headerColor.b, _headerColor.a);
	SDL_RenderFillRect(_renderer, &_headerRect);
	SDL_RenderFillRect(_renderer, &_footerRect);

	SDL_SetRenderDrawColor(_renderer, _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
	SDL_RenderDrawRect(_renderer, &_backgroudRect);

	SDL_SetRenderDrawColor(_renderer, _borderColor.r, _borderColor.g, _borderColor.b, _borderColor.a);
	SDL_RenderDrawRect(_renderer, &_headerRect);
	SDL_RenderDrawRect(_renderer, &_footerRect);
}

void PauseScreen::_drawBrowser()
{
	SDL_RenderCopy(_renderer, _textureDir, NULL, &_rectDir);

	SDL_Rect selectedLineBackground = _selectedLineBackground;
	selectedLineBackground.y += _selectedLineIndex * 20;
	SDL_SetRenderDrawColor(_renderer, _selectedLineBackgroundColor.r, _selectedLineBackgroundColor.g, _selectedLineBackgroundColor.b, _selectedLineBackgroundColor.a);
	SDL_RenderFillRect(_renderer, &selectedLineBackground);

    int line = 0;
    for (std::pair<SDL_Rect, SDL_Texture*> & file : _filesList)
    {
        SDL_RenderCopy(_renderer, file.second, NULL, &file.first);
    }
}

void PauseScreen::_drawOption()
{

}

void PauseScreen::setRomPath(string newPath)
{
	stringstream ss;
	ss << "Dir : " << newPath;
	string str = _getShortPath(ss.str());
    _rectDir.x = PAUSE_SCREEN_OFFSET + 4;
    _rectDir.y = (PAUSE_SCREEN_OFFSET / 2) + 6;
	_buildTexture(str, &_textureDir, &_rectDir);
}

void PauseScreen::setFilesList(const vector<File> & list)
{
	_filesList.clear();

    int offset = 20;
    for (const File & file : list)
    {
        SDL_Rect rect = { PAUSE_SCREEN_OFFSET + 4,  ((PAUSE_SCREEN_OFFSET / 2) + 15) + offset, 0, 0 };
        SDL_Texture * texture = nullptr;
        _buildTexture(file.fileName, &texture, &rect);
        _filesList.push_back(pair<SDL_Rect, SDL_Texture*>(rect, texture));
        offset += 20;
    }
}

void PauseScreen::_initText()
{
	_font = TTF_OpenFont("cour.ttf", 15);

	if (_font == nullptr)
		throw TextFontException("Can't load text font file cour.ttf");

	setRomPath("");
}

void PauseScreen::_buildTexture(const string & str, SDL_Texture ** texture, SDL_Rect * rec)
{
	if (*texture != nullptr)
		SDL_DestroyTexture(*texture);
    *texture = nullptr;

	SDL_Surface * surface = TTF_RenderText_Solid(_font, str.c_str(), { 255, 255, 255 });
	*texture = SDL_CreateTextureFromSurface(_renderer, surface);
	int w, h;
	SDL_QueryTexture(*texture, NULL, NULL, &w, &h);
	*rec = { rec->x, rec->y, w, h };
    SDL_FreeSurface(surface);
}

string PauseScreen::_getShortPath(string & path)
{
	if (path.length() <= PATH_MAX_LENGTH)
		return path;

	size_t indexLastOfSlash = path.find_last_of('\\');
	string secondPart = path.substr(indexLastOfSlash);

	// +3 because au "..."
	size_t leftSize = (PATH_MAX_LENGTH + 3) - secondPart.length();
	string firstPart = path.substr(0, leftSize);

	stringstream ss;
	ss << firstPart << "..." << secondPart;
	return ss.str();
}

int PauseScreen::getSelectedLineIndex() const
{
	return _selectedLineIndex;
}

void PauseScreen::setSelectedLineIndex(int value)
{
	_selectedLineIndex = value;
}