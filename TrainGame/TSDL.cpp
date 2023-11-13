#include "TSDL.h"


TSDL::TSDL()
{
}

SDL_Texture* TSDL::CreateTexture(const char* path, const Color KeyColor)
{
	SDL_Surface* surface = IMG_Load(path);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, KeyColor.r, KeyColor.g, KeyColor.b));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture* TSDL::CreateTexture(const char* path, const SDL_Color KeyColor)
{
	SDL_Surface* surface = IMG_Load(path);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, KeyColor.r, KeyColor.g, KeyColor.b));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture* TSDL::CreateTextureToFont(TTF_Font* font, const char* text, const SDL_Color fg)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, fg);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture* TSDL::CreateTextureToFont(TTF_Font* font, const char* text, const SDL_Color fg, SDL_Rect& rect)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, fg);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
	
	rect = { 0, 0, surface->w, surface->h };

	SDL_FreeSurface(surface);

	return CreateTextureToFont(font, text, fg);
}

SDL_Texture* TSDL::CreateTexture(const char* path)
{
	return CreateTexture(path, Color());
}



Color::Color()
{
	r = 255;
	g = 255;
	b = 255;
}

Color::Color(int r, int g, int b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}
