#pragma once

#include <SDL2/SDL.h>

class Drawable
{
public:
	/* Call drawElement if _visible is true */
	void draw(SDL_Renderer * renderer);
	/* Call clearElement if _visible is true */
	void clear(SDL_Renderer * renderer);
	virtual void drawElement(SDL_Renderer * renderer) = 0;
	virtual void clearElement(SDL_Renderer * renderer);

	bool isVisible() const;
	void setVisible(bool value);

protected:
	bool _visible;
};