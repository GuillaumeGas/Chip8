#include "Drawable.hpp"

Drawable::Drawable(SDL_Renderer * renderer) : _renderer(renderer), _visible(true) {}

void Drawable::draw()
{
	if (_visible)
		drawElement();
}

bool Drawable::isVisible() const
{
	return _visible;
}

void Drawable::setVisible(bool value)
{
	_visible = value;
}