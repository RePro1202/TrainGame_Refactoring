#include "Core.h"

void InitGame() {
	g_flag_running = true;
	g_current_game_phase = PHASE_INTRO;
	g_day = DAY_MORNING;
	g_time_hour = 0;
	g_time_min = 0;
	g_time_sec = 0;
	g_train_pos = 372;
	g_score = 0;
	g_goal_time_update = false;
	g_time_update = false;
	g_score_update = true;
	g_train_pos_update = false;
	g_bg_music = Mix_LoadMUS("../../Resources/Intro_train.mp3");
	g_train_run_sound = Mix_LoadWAV("../../Resources/running_train.wav");
	g_train_start_sound = Mix_LoadWAV("../../Resources/starting_train.wav");
}

void ClearGame() {
	Mix_FreeMusic(g_bg_music);
	Mix_FreeChunk(g_train_run_sound);
	Mix_FreeChunk(g_train_start_sound);
}

//// UserInterface////
PhaseInterface::PhaseInterface()
{
	// 시간 빠르기
	time_speed_ = 10;
	// 목표시간
	goal_morning_h_ = 1;
	goal_morning_m_ = 30;
	goal_daytime_h_ = 3;
	goal_daytime_m_ = 00;
	goal_evening_h_ = 4;
	goal_evening_m_ = 30;
	// 시작 시간 조정
	g_time_hour = 0;
	g_time_min = 0;
	g_time_sec = g_time_min * time_speed_;

	goal_time_hour_ = goal_morning_h_;
	goal_time_min_ = goal_morning_m_;

	// 시간 폰트
	UI_font_ = TTF_OpenFont("../../Resources/RobotoMono.ttf", 100);
	ending_font_ = TTF_OpenFont("../../Resources/SHOWG.ttf", 100);
	darkblue_ = { 0,0,150,0 };
	yellow_ = { 255,242,0,0 };
	black_ = { 0,0,0,0 };
	// 도착 시간
	sprintf_s(buf_, "%02d:%02d", goal_time_hour_, goal_time_min_);
	goal_time_char_ = buf_;
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(UI_font_, goal_time_char_, darkblue_);
	goal_time_rect_ = { 0, 0, tmp_surface->w, tmp_surface->h };
	goal_time_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	// 현재 시간
	sprintf_s(buf_, "%02d:%02d", g_time_hour, g_time_min);
	time_char_ = buf_;
	SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(UI_font_, time_char_, darkblue_);
	time_rect_ = { 0, 0, tmp_surface2->w, tmp_surface2->h };
	time_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
	SDL_FreeSurface(tmp_surface2);
	// Score
	sprintf_s(buf2_, "%04d", g_score);
	score_char_ = buf2_;
	SDL_Surface* tmp_surface3 = TTF_RenderText_Blended(UI_font_, score_char_, black_);
	score_rect_ = { 0, 0, tmp_surface3->w, tmp_surface3->h };
	score_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface3);
	SDL_FreeSurface(tmp_surface3);

	SDL_Surface* ui_surface = IMG_Load("../../Resources/UI.png");
	SDL_SetColorKey(ui_surface, SDL_TRUE, SDL_MapRGB(ui_surface->format, 200, 200, 250));
	ui_texture_ = SDL_CreateTextureFromSurface(g_renderer, ui_surface);
	SDL_FreeSurface(ui_surface);

	// Time
	ui_source_rectangle_[0] = { 9, 7, 192, 131 };
	ui_destination_rectangle_[0] = { 50, 20, 220, 140 };

	// Score
	ui_source_rectangle_[1] = { 215, 40, 206, 56 };
	ui_destination_rectangle_[1] = { 950, 20, 200, 120 };

	// Speed
	ui_source_rectangle_[2] = { 436, 65, 240, 71 };
	ui_destination_rectangle_[2] = { 950, 160, 200, 100 };

	// Train_map
	ui_source_rectangle_[3] = { 12, 151, 564, 143 };
	ui_destination_rectangle_[3] = { 340, 20, 520, 140 };

	// Train_pos
	ui_source_rectangle_[4] = { 50, 318, 38, 15 };
	ui_destination_rectangle_[4] = { 372, 67, 40, 20 };

	//페이드
	SDL_Surface* black_surface = IMG_Load("../../Resources/black.png");
	black_texture_ = SDL_CreateTextureFromSurface(g_renderer, black_surface);
	SDL_FreeSurface(black_surface);
	black_source_rect_ = { 0, 0 ,1200 ,800 };
	black_destination_rect_ = { 0, 0, black_source_rect_.w, black_source_rect_.h };
	alpha_ = 0;
}

PhaseInterface::~PhaseInterface()
{
	TTF_CloseFont(UI_font_);
	SDL_DestroyTexture(ui_texture_);
	SDL_DestroyTexture(time_texture_);
	SDL_DestroyTexture(black_texture_);
}

void PhaseInterface::ShowUI()
{
	for (int i = 0; i < 5; i++) {
		SDL_RenderCopy(g_renderer, ui_texture_, &ui_source_rectangle_[i], &ui_destination_rectangle_[i]);
	}
}

void PhaseInterface::SetGoalTimeFont()
{
	// 목표시간 조정
	switch (g_day)
	{
	case DAY_MORNING:
		goal_time_hour_ = goal_morning_h_;
		goal_time_min_ = goal_morning_m_;
		break;
	case DAY_DAYTIME:
		if (g_current_game_phase == PHASE_PLATFORM)
		{
			goal_time_hour_ = goal_morning_h_;
			goal_time_min_ = goal_morning_m_;
		}
		else
		{
			goal_time_hour_ = goal_daytime_h_;
			goal_time_min_ = goal_daytime_m_;
		}
		break;
	case DAY_EVENING:
		if (g_current_game_phase == PHASE_PLATFORM)
		{
			goal_time_hour_ = goal_daytime_h_;
			goal_time_min_ = goal_daytime_m_;
		}
		else
		{
			goal_time_hour_ = goal_evening_h_;
			goal_time_min_ = goal_evening_m_;
		}
		break;
	case DAY_NIGHT:
		goal_time_hour_ = goal_evening_h_;
		goal_time_min_ = goal_evening_m_;
		break;
	}
	SDL_DestroyTexture(goal_time_texture_);
	sprintf_s(buf_, "%02d:%02d", goal_time_hour_, goal_time_min_);
	goal_time_char_ = buf_;
	SDL_Surface* tmp_surface = TTF_RenderText_Blended(UI_font_, goal_time_char_, darkblue_);
	goal_time_rect_ = { 0, 0, tmp_surface->w, tmp_surface->h };
	goal_time_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
}

void PhaseInterface::SetTimeFont()
{
	SDL_DestroyTexture(time_texture_);
	sprintf_s(buf_, "%02d:%02d", g_time_hour, g_time_min);
	time_char_ = buf_;
	SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(UI_font_, time_char_, darkblue_);
	time_rect_ = { 0, 0, tmp_surface2->w, tmp_surface2->h };
	time_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
	SDL_FreeSurface(tmp_surface2);
}

void PhaseInterface::TimeUpdate()
{
	++g_time_sec;
	if (g_time_sec == 60 * time_speed_)
	{
		g_time_sec = 0;
		++g_time_hour;
		if (g_time_hour == 24)
			g_time_hour = 0;
	}
	g_time_min = g_time_sec / time_speed_;
}

void PhaseInterface::TrainPosUpdate()
{
	g_train_pos += 68;
	ui_destination_rectangle_[4].x = g_train_pos;
}

void PhaseInterface::SetScoreFont()
{
	SDL_DestroyTexture(score_texture_);
	sprintf_s(buf2_, "%04d", g_score);
	score_char_ = buf2_;
	SDL_Surface* tmp_surface3 = TTF_RenderText_Blended(UI_font_, score_char_, black_);
	score_rect_ = { 0, 0, tmp_surface3->w, tmp_surface3->h };
	score_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface3);
	SDL_FreeSurface(tmp_surface3);
}

void PhaseInterface::IncreaseScore(int score)
{
	g_score += score;
}

void PhaseInterface::DecreaseScore(int score)
{
	g_score -= score;
	if (g_score < 0)
		g_score = 0;
}

int PhaseInterface::GoalSuccess()
{
	if (g_time_hour < goal_time_hour_)
		return 1;
	else if (g_time_hour == goal_time_hour_)
	{
		if (g_time_min <= goal_time_min_)
			return 1;
		else
			return 0;

	}
	else
		return 0;
}

void PhaseInterface::SetEndingTimeFont()
{
	SDL_DestroyTexture(time_texture_);
	sprintf_s(buf_, "TIME : %02d:%02d", g_time_hour, g_time_min);
	time_char_ = buf_;
	SDL_Surface* tmp_surface2 = TTF_RenderText_Blended(ending_font_, time_char_, yellow_);
	time_rect_ = { 0, 0, tmp_surface2->w, tmp_surface2->h };
	time_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface2);
	SDL_FreeSurface(tmp_surface2);
}

void PhaseInterface::SetEndingScoreFont()
{
	SDL_DestroyTexture(score_texture_);
	sprintf_s(buf2_, "SCORE : %04d", g_score);
	score_char_ = buf2_;
	SDL_Surface* tmp_surface3 = TTF_RenderText_Blended(ending_font_, score_char_, yellow_);
	score_rect_ = { 0, 0, tmp_surface3->w, tmp_surface3->h };
	score_texture_ = SDL_CreateTextureFromSurface(g_renderer, tmp_surface3);
	SDL_FreeSurface(tmp_surface3);
}

// FadeIn(), FadeOut() --> 렌더에서 호출
// EndFade() --> 알파값 초기화, 페이드 종료 시점에서 호출해야함
void PhaseInterface::FadeIn() {

	SDL_SetTextureAlphaMod(black_texture_, 255 - alpha_);
	SDL_RenderCopy(g_renderer, black_texture_, &black_source_rect_, &black_destination_rect_);

	if (alpha_ < 255) {
		alpha_ += 5;
	}
}

void PhaseInterface::FadeOut(int i) {

	int a = i;
	SDL_SetTextureAlphaMod(black_texture_, alpha_);
	SDL_RenderCopy(g_renderer, black_texture_, &black_source_rect_, &black_destination_rect_);

	if (alpha_ < 255) {
		alpha_ += a;
	}
}

void PhaseInterface::EndFade() {
	alpha_ = 0;
}