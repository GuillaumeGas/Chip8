#include "Drawable.hpp"

void Drawable::draw(SDL_Renderer * renderer)
{
	if (_visible)
		drawElement(renderer);
}

void Drawable::clear(SDL_Renderer * renderer)
{
	if (_visible)
		clearElement(renderer);
}

void Drawable::clearElement(SDL_Renderer * renderer)
{
}

bool Drawable::isVisible() const
{
	return _visible;
}

void Drawable::setVisible(bool value)
{
	_visible = value;
}