#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Screen.hpp"
#include "Drawable.hpp"
#include "Utils\Directory.hpp"

#define PAUSE_SCREEN_OFFSET 50
#define HEADER_HEIGHT 30

#define PATH_MAX_LENGTH 50

class PauseScreen : public Drawable
{
public:
	PauseScreen(SDL_Renderer * renderer);
	~PauseScreen();

	enum ScreenType
	{
		BROWSE_SCREEN,
		OPTION_SCREEN,
	};

	void drawElement() override;
	void setRomPath(std::string newPath);
    void setFilesList(const std::vector<File> & list);
	void setScreenType(ScreenType type);
	int getSelectedLineIndex() const;
	void setSelectedLineIndex(int value);

protected:
	void _initText();
	void _drawBackground();
	void _drawBrowser();
	void _drawOption();
	void _buildTexture(const std::string & str, SDL_Texture ** texture, SDL_Rect * rec);
	std::string _getShortPath(std::string & path);

	ScreenType _screenType;
	TTF_Font * _font;
	SDL_Texture * _textureDir;
	SDL_Rect _rectDir;
    std::vector<std::pair<SDL_Rect, SDL_Texture*>> _filesList;
    int _selectedLineIndex;

	const SDL_Rect _backgroudRect
	{
		PAUSE_SCREEN_OFFSET,
		PAUSE_SCREEN_OFFSET / 2,
		(SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
		(SCREEN_HEIGHT * PIXEL_DIM) - PAUSE_SCREEN_OFFSET,
	};

	const SDL_Rect _headerRect
	{
		PAUSE_SCREEN_OFFSET,
		PAUSE_SCREEN_OFFSET / 2,
		(SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
		HEADER_HEIGHT,
	};

	const SDL_Rect _footerRect
	{
		PAUSE_SCREEN_OFFSET,
		(SCREEN_HEIGHT * PIXEL_DIM) - ((PAUSE_SCREEN_OFFSET / 2) + HEADER_HEIGHT),
		(SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2,
		HEADER_HEIGHT,
	};

    const SDL_Rect _selectedLineBackground
    {
        PAUSE_SCREEN_OFFSET + 2,
        ((PAUSE_SCREEN_OFFSET / 2) + 6) + 30,
        ((SCREEN_WIDTH * PIXEL_DIM) - PAUSE_SCREEN_OFFSET * 2) - 2,
        HEADER_HEIGHT / 2,
    };

	const SDL_Color _backgroundColor { 0, 102, 0, 150 };
	const SDL_Color _borderColor { 0, 153, 0, 255 };
	const SDL_Color _headerColor{ 2, 73, 2, 170 };
    const SDL_Color _selectedLineBackgroundColor { 0, 153, 0, 100 };
};