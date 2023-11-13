#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "SDL_image.h"

#define BLOCK_X_MAX 1200
#define BLOCK_Y_MAX 800

void InitGame();
void ClearGame();

// 게임 페이즈(열거체를 사용)
enum Phase
{
	PHASE_INTRO,
	PHASE_PLATFORM,
	PHASE_RUNNING,
	PHASE_ENDING
};

enum BackGrounf
{
	BG_INTRO,
	BG_ENDING
};

enum Day
{
	DAY_MORNING,
	DAY_DAYTIME,
	DAY_EVENING,
	DAY_NIGHT
};

// 따로 객체를 생성하지 않고 사용해야하므로 전역변수로 설정
extern int g_current_game_phase;
extern int g_day;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern int g_time_hour;
extern int g_time_min;
extern int g_time_sec;
extern int g_train_pos;
extern int g_score;
extern bool g_goal_time_update;
extern bool g_time_update;
extern bool g_score_update;
extern bool g_train_pos_update;
extern Mix_Music* g_bg_music;
extern Mix_Chunk* g_train_run_sound;
extern Mix_Chunk* g_train_start_sound;


class PhaseInterface
{
private:
	// ui 전체 이미지로드 변수
	SDL_Texture* ui_texture_;
	SDL_Rect ui_source_rectangle_[5];
	SDL_Rect ui_destination_rectangle_[5];
	// Font 관련 변수
	TTF_Font* UI_font_;
	TTF_Font* ending_font_;
	SDL_Color darkblue_;
	SDL_Color yellow_;
	SDL_Color black_;
	// Time 관련 변수
	SDL_Rect time_rect_;
	SDL_Texture* time_texture_;
	const char* time_char_;
	char buf_[256];
	int time_speed_;
	// 목표시간 관련 변수
	SDL_Rect goal_time_rect_;
	SDL_Texture* goal_time_texture_;
	const char* goal_time_char_;
	int goal_time_hour_;
	int goal_time_min_;
	int goal_morning_h_;
	int goal_daytime_h_;
	int goal_evening_h_;
	int goal_morning_m_;
	int goal_daytime_m_;
	int goal_evening_m_;
	SDL_Texture* black_texture_;
	SDL_Rect black_source_rect_;
	SDL_Rect black_destination_rect_;
	int alpha_;
	// Score 관련 변수
	SDL_Texture* score_texture_;
	SDL_Rect score_rect_;
	const char* score_char_;
	char buf2_[256];

public:
	PhaseInterface();
	~PhaseInterface();

	void ShowUI();
	// Time 관련 함수
	void SetTimeFont();
	void TimeUpdate();
	SDL_Texture* GetTimeTexture() { return time_texture_; };
	SDL_Rect GetTimeRect() { return time_rect_; };
	// 도착시간 관련 함수
	void SetGoalTimeFont();
	SDL_Texture* GetGoalTimeTexture() { return goal_time_texture_; };
	SDL_Rect GetGoalTimeRect() { return goal_time_rect_; };
	// TrainPos 관련 함수
	void TrainPosUpdate();
	// Score 관련 함수
	void SetScoreFont();
	void IncreaseScore(int score);
	void DecreaseScore(int score);
	int GoalSuccess();
	SDL_Texture* GetScoreTexture() { return score_texture_; };
	SDL_Rect GetScoreRect() { return score_rect_; };
	// Ending
	void SetEndingTimeFont();
	void SetEndingScoreFont();

	//페이드 인, 아웃
	void FadeIn();
	void FadeOut(int i);
	void EndFade();

	virtual void HandleEvents() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};