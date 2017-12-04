#include "PauseScreen.hpp"

void PauseScreen::drawElement(SDL_Renderer * renderer)
{
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 100);
	SDL_RenderFillRect(renderer, &_rect);
}