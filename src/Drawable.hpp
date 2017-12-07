#pragma once

#include <SDL2/SDL.h>

class Drawable
{
public:
	Drawable(SDL_Renderer * renderer);

	/* Call drawElement if _visible is true */
	void draw();
	virtual void drawElement() = 0;

	bool isVisible() const;
	void setVisible(bool value);

protected:
	bool _visible;
	SDL_Renderer * _renderer;
};