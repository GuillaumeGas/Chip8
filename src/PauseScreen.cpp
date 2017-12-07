#include "PauseScreen.hpp"
#include <string>
#include "Utils\Directory.hpp"

using namespace std;

PauseScreen::PauseScreen(SDL_Renderer * renderer) : Drawable(renderer), _font(nullptr), _surface(nullptr), _textureDir(nullptr)
{
	TTF_Init();
	_initText();
}

PauseScreen::~PauseScreen()
{
	if (_textureDir != nullptr)
		SDL_DestroyTexture(_textureDir);
	SDL_FreeSurface(_surface);
	TTF_CloseFont(_font);
	TTF_Quit();
}

void PauseScreen::drawElement()
{
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

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

	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);

	SDL_RenderCopy(_renderer, _textureDir, NULL, &_rectDir);
}

void PauseScreen::setRomPath(string newPath)
{
	stringstream ss;
	ss << "Dir : " << newPath;
	string str = _getShortPath(ss.str());
	_buildTexture(str, _textureDir);
}

void PauseScreen::_initText()
{
	_font = TTF_OpenFont("cour.ttf", 15);

	if (_font == nullptr)
		throw TextFontException("Can't load text font file cour.ttf");

	setRomPath("");
}

void PauseScreen::_buildTexture(string & str, SDL_Texture * texture)
{
	if (texture != nullptr)
		SDL_DestroyTexture(_textureDir);
	_textureDir = nullptr;

	_surface = TTF_RenderText_Solid(_font, str.c_str(), { 255, 255, 255 });
	_textureDir = SDL_CreateTextureFromSurface(_renderer, _surface);
	int w, h;
	SDL_QueryTexture(_textureDir, NULL, NULL, &w, &h);
	_rectDir = { PAUSE_SCREEN_OFFSET + 4, (PAUSE_SCREEN_OFFSET / 2) + 6, w, h };
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