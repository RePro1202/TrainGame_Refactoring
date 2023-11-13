#pragma once
#include "Core.h"

static class TSDL
{
public:
	TSDL();

	static SDL_Texture* CreateTexture(const char* path);
	static SDL_Texture* CreateTexture(const char* path, const class Color KeyColor);
	static SDL_Texture* CreateTexture(const char* path, const SDL_Color KeyColor);

	static SDL_Texture* CreateTextureToFont(TTF_Font* font, const char* text, const SDL_Color fg);
	static SDL_Texture* CreateTextureToFont(TTF_Font* font, const char* text, const SDL_Color fg, SDL_Rect& rect);
};

class Color
{
public:
	int r;
	int g;
	int b;

public:
	Color();
	Color(int r, int g, int b);
};