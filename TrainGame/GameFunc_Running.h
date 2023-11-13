#pragma once

#include "Core.h"
#include "GameFunc_Events.h"
#include "time.h"

class Running : public PhaseInterface
{
private:
	SDL_Texture* background_texture_[3];
	SDL_Rect background_source_rect_[3];
	SDL_Rect background_destination_rect_[3];
	SDL_Texture* track_texture_;
	SDL_Rect track_source_rect_;
	SDL_Rect track_destination_rect_;
	SDL_Texture* train_texture_;
	SDL_Rect train_source_rect_;
	SDL_Rect train_destination_rect_;
	SDL_Texture* arrow_texture_;
	SDL_Rect arrow_source_rect_;
	SDL_Rect arrow_destination_rect_;

	int train_distance_;
	double train_speed_;
	double arrow_speed_; //ȭ��ǥ �ӵ��� ���ؼ� �����е�� ���� �� ������ �����̶� �ϴ� double������ �׽��ϴ�..
	Events* eve_;
	EventScore* e_score;

	bool RightKey;

public:
	Running();
	~Running();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;
};