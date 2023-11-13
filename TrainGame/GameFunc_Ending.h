#pragma once

#include "Core.h"

class Ending : public PhaseInterface
{
private:
	SDL_Texture* ending_out_texture_;
	SDL_Rect ending_out_rect_;
	SDL_Texture* ending_title_texture_;
	SDL_Rect ending_title_rect_;
	TTF_Font* ending_font_;
	SDL_Color black_;
	SDL_Rect board_rect_;
	bool render_complete;
public:
	Ending();
	~Ending();

	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;
};