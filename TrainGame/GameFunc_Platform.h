#pragma once

#include "Core.h"

enum TrainState
{
	TRAIN_IN,
	TRAIN_STOP,
	TRAIN_OUT
};

class Platform : public PhaseInterface
{
private:
	SDL_Texture* platform_texture_;
	SDL_Rect platform_source_rect_;
	SDL_Rect platform_destination_rect_;

	SDL_Texture* train_texture_;
	SDL_Rect train_source_rect_;
	SDL_Rect train_destination_rect_;
	int train_state_, train_speed_, stop_destination_, consumption_time_;

	/////////////////////////
	SDL_Texture* train2_texture_;
	SDL_Rect train2_source_rect_half_;
	SDL_Rect train2_source_rect_;
	SDL_Rect train2_destination_rect_;
	int train2_count_;

	SDL_Texture* sign_texture_;
	SDL_Rect sign_source_rect_good_;
	SDL_Rect sign_source_rect_fail_;
	SDL_Rect sign_destination_rect_;
	int sign_count_, sign_time_;

	SDL_Texture* people_texture_;
	SDL_Rect people_source_rect_right_;
	SDL_Rect people_source_rect_stop_;
	SDL_Rect people_source_rect_left_;
	SDL_Rect people_destination_rect_;


	int people_stop_destination_[5];
	int people_count, random_count_, random2_count_, door_count_;// 사람수 
	int people_destination_Array_[30][5]; // [사람수]
	//[0 상태1:x좌표 2:y좌표 3: 문 번호 ] 

	SDL_Texture* door_texture_;
	SDL_Rect door_source_rect_right_;
	SDL_Rect door_source_rect_left_;
	SDL_Rect door_destination_rect_;
	int door_stop_destination_[10][2];   // 문갯수 /   0 : x  , 1 : y;
	///
	int score_count_;
public:
	Platform();
	~Platform();
	virtual void HandleEvents() override;
	virtual void Update() override;
	virtual void Render() override;
};