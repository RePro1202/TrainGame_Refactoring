#include "GameFunc_Platform.h"
#include <cstdlib>
#include <ctime>

Platform::Platform()
{
	SDL_Surface* platform_surface = IMG_Load("../../Resources/platform.png");
	SDL_SetColorKey(platform_surface, SDL_TRUE, SDL_MapRGB(platform_surface->format, 200, 191, 231));
	platform_texture_ = SDL_CreateTextureFromSurface(g_renderer, platform_surface);
	SDL_FreeSurface(platform_surface);
	platform_source_rect_ = { 0, 0 ,1200 ,800 };
	platform_destination_rect_ = { 0, 0, platform_source_rect_.w, platform_source_rect_.h };

	SDL_Surface* train_surface = IMG_Load("../../Resources/train.png");
	SDL_SetColorKey(train_surface, SDL_TRUE, SDL_MapRGB(train_surface->format, 100, 170, 150));
	train_texture_ = SDL_CreateTextureFromSurface(g_renderer, train_surface);
	SDL_FreeSurface(train_surface);
	train_source_rect_ = { 50, 50 ,1800 ,250 };
	train_destination_rect_ = { -1600 , 380, train_source_rect_.w, train_source_rect_.h };

	SDL_Surface* train2_surface = IMG_Load("../../Resources/train2.png");
	SDL_SetColorKey(train2_surface, SDL_TRUE, SDL_MapRGB(train2_surface->format, 100, 170, 150));
	train2_texture_ = SDL_CreateTextureFromSurface(g_renderer, train2_surface);
	SDL_FreeSurface(train2_surface);
	train2_source_rect_half_ = { 50, 50 ,1800 ,250 };
	train2_source_rect_ = { 50, 257 ,1800 ,250 };
	train2_destination_rect_ = { -773 , 380, train2_source_rect_.w, train2_source_rect_.h };

	SDL_Surface* people_surface = IMG_Load("../../Resources/people.png");
	people_texture_ = SDL_CreateTextureFromSurface(g_renderer, people_surface);
	SDL_FreeSurface(people_surface);

	SDL_Surface* sign_surface = IMG_Load("../../Resources/signal.png");
	sign_texture_ = SDL_CreateTextureFromSurface(g_renderer, sign_surface);
	sign_source_rect_fail_ = { 229, 268 ,773 ,784 }; //  fail
	sign_source_rect_good_ = { 1141,268  ,773 ,784 }; // good
	sign_count_ = 0;
	SDL_FreeSurface(sign_surface);

	SDL_Surface* door_surface = IMG_Load("../../Resources/door.png");
	door_texture_ = SDL_CreateTextureFromSurface(g_renderer, door_surface);
	SDL_FreeSurface(door_surface);


	people_source_rect_right_ = { 169, 283 ,700 ,1100 }; // 우측으로 걷는사람
	people_source_rect_left_ = { 1157, 291 ,700 ,1100 }; // 좌측으로 걷는사람
	people_source_rect_stop_ = { 160, 1554 ,700 ,1100 }; // 멈춘 사람
	people_destination_rect_ = { 100 ,500,60,84 }; // 사람크기 임시 60 84
	door_source_rect_left_ = { 211, 195, 41 ,110 }; // 문 왼쪽
	door_source_rect_right_ = { 269, 195,42,110 }; // 문 오른쪽
	door_destination_rect_ = { 500 ,500,45,110 }; // 임시 좌표값, 문의 크기 45 110
	sign_destination_rect_ = { 600 ,250,100,100 };

	score_count_ = 0;
}

void Platform::Update()
{
	// 목표시간 안에 도착하면 스코어증가
	if (PhaseInterface::GoalSuccess() == 1 && g_day != DAY_MORNING && score_count_ == 0)
	{
		PhaseInterface::IncreaseScore(800);
		g_score_update = true;
		score_count_++;
	}
	else if (PhaseInterface::GoalSuccess() == 0 && g_day != DAY_MORNING && score_count_ == 0)
	{
		PhaseInterface::DecreaseScore(500);
		g_score_update = true;
		score_count_++;
	}

	if (train_state_ == TRAIN_IN)
	{
		if (train_destination_rect_.x < -1599)  // 초기화 부분 
		{
			door_count_ = 42; // 문이 움직일 변수
			train2_count_ = 4;
			for (int i = 0; i < 5; i++) // 문 위치
			{
				door_stop_destination_[i * 2][0] = 83 + i * 168;   // 왼쪽 문 위치
				door_stop_destination_[i * 2][1] = 438;
				door_stop_destination_[i * 2 + 1][0] = 125 + i * 168; // 오른쪽 문 위치
				door_stop_destination_[i * 2 + 1][1] = 438;
			}

			people_count = 0;

			if (g_day != DAY_NIGHT) {
				people_count = 10;// 출현 할 사람수 
				random_count_ = 30; // 배열로 지정된 사람 최대수
				random2_count_ = people_count;
				srand((unsigned int)time(NULL)); // 랜덤
				for (int i = 0; i < 5; i++)
				{
					people_stop_destination_[i] = 0; // 해당 문의 차례
				}
				for (int i = 0; i < 6; i++)
				{
					for (int k = 0; k < 5; k++)
					{
						if (rand() % random_count_ <= random2_count_ && random2_count_ != 0)  // 조건문 만족시 5 * 6 중 해당 k 행 i 열에 임의의 범위에 사람 소환  
						{
							random2_count_--;
							people_destination_Array_[i * 5 + k][0] = 1; // 1 이상일때 행동 중 // 1은 움직이기 전 상태
							people_destination_Array_[i * 5 + k][1] = 20 + rand() % 170 + 170 * k; // 랜덤 좌표
							people_destination_Array_[i * 5 + k][2] = 500 + (rand() % 20) + 20 * i; //렌덤 좌표
							people_destination_Array_[i * 5 + k][3] = ((int)(people_destination_Array_[i * 5 + k][1] - 20) / 170);
							people_destination_Array_[i * 5 + k][4] = people_stop_destination_[people_destination_Array_[i * 5 + k][3]]; // 자신과 가까운 문의 차레를 받음
							people_stop_destination_[people_destination_Array_[i * 5 + k][3]] += 1;  // 자신과 가까운 문의 차례 + 1
							//ctime과 cstdilb헤더파일을 사용했으니 정리 바람...[0 상태1:x좌표 2:y좌표 3: 문 번호 4: 차례]
						}
						else {
							people_destination_Array_[i * 5 + k][0] = 0; //조건문 만족 못한 사람은 작동안하도록
						}
						random_count_--;
					}
				}
			}


			train_state_ = TRAIN_IN;
			train_speed_ = 0;
			stop_destination_ = -775;
			consumption_time_ = 3; // 정지까지 필요한 시간
		}
		train_speed_ = (2 * (stop_destination_ - train_destination_rect_.x)) / (30 * consumption_time_) + 2;
		if (train_destination_rect_.x > stop_destination_) {
			train_destination_rect_.x = stop_destination_;
			train_state_ = TRAIN_STOP;
		}

		for (int i = 0; i < 30; i++) // 가까운 문과 아까 계산한 해당문에 관한 자신 차례의 좌표로 이동
		{
			if (people_destination_Array_[i][0] != 0 && people_destination_Array_[i][0] != 4)
			{
				if (people_destination_Array_[i][1] == people_destination_Array_[i][3] * 170 + 90 + 1 || people_destination_Array_[i][1] == people_destination_Array_[i][3] * 170 + 90 - 1) {
					people_destination_Array_[i][1] = people_destination_Array_[i][3] * 170 + 90;
				}
				else if (people_destination_Array_[i][1] < people_destination_Array_[i][3] * 170 + 90) {
					people_destination_Array_[i][0] = 3;
					people_destination_Array_[i][1] += 2;
				}
				else if (people_destination_Array_[i][1] > people_destination_Array_[i][3] * 170 + 90)
				{
					people_destination_Array_[i][0] = 2;
					people_destination_Array_[i][1] -= 2;
				}

				if (people_destination_Array_[i][2] < people_destination_Array_[i][4] * 20 + 500) {
					people_destination_Array_[i][2] += 1;
				}
				else if (people_destination_Array_[i][2] > people_destination_Array_[i][4] * 20 + 500) {
					people_destination_Array_[i][2] -= 1;
				}
			}
			if (people_destination_Array_[i][1] == people_destination_Array_[i][3] * 170 + 90 && people_destination_Array_[i][2] == people_destination_Array_[i][4] * 20 + 500 && people_destination_Array_[i][0] != 4) {
				people_destination_Array_[i][0] = 4;
			}
		}
	}
	else if (train_state_ == TRAIN_STOP && g_day != DAY_NIGHT)
	{
		train_speed_ = 0;
		consumption_time_ = 2; //나갈때 까지 걸리는 시간

		if (door_count_ > 0 && door_count_ < 43) {
			for (int i = 0; i < 5; i++) { // 문 열림
				door_stop_destination_[i * 2][0] -= 2;
				door_stop_destination_[i * 2 + 1][0] += 2;
			}
			door_count_ -= 2;

		}
		else if (door_count_ == 0 && sign_time_ == 0) {
			for (int i = 0; i < 30; i++) {
				people_destination_Array_[i][2] -= 2;
				if (people_destination_Array_[i][2] < 450 && people_destination_Array_[i][0] != 0) {
					people_destination_Array_[i][0] = 0;
					people_count--;

					// 승객이 들어갈때 작동되는 부분
				}
			}
			if (people_count <= 0)
			{
				door_count_ = 44;
			}
		}

		if (door_count_ > 21 && door_count_ < 43) {
			train2_count_ = 1;
		}
		else if ((door_count_ >= 0 && door_count_ <= 20) || (door_count_ >= 44 && door_count_ <= 65)) {
			train2_count_ = 0;
		}
		else if (door_count_ > 65 && door_count_ <= 85) {
			train2_count_ = 1;
		}

		if (door_count_ > 43 && door_count_ < 86) {
			for (int i = 0; i < 5; i++)
			{ // 문 닫힘
				door_stop_destination_[i * 2][0] += 2;
				door_stop_destination_[i * 2 + 1][0] -= 2;
			}
			door_count_ += 2;
		}
		else if (door_count_ == 86) {
			door_count_ = 87; // 정지상태
			train2_count_ = 4;
		}
		PhaseInterface::EndFade();
	}
	else if (train_state_ == TRAIN_OUT && g_day != DAY_NIGHT) {
		train_speed_ = 2 * (BLOCK_X_MAX - stop_destination_) / (30 * consumption_time_) + 2
			- (2 * (BLOCK_X_MAX - train_destination_rect_.x)) / (30 * consumption_time_);
		// 화면 밖으로 나가면 running페이즈로 전환(변수 초기화)
		if (train_destination_rect_.x > BLOCK_X_MAX)
		{
			g_current_game_phase = PHASE_RUNNING;

			PhaseInterface::EndFade();

			train_destination_rect_.x = -1600;
			train_state_ = TRAIN_IN;
			train_speed_ = 0;
			consumption_time_ = 3;
			score_count_ = 0;

			g_time_update = true;
			g_score_update = true;
			g_train_pos_update = true;
			g_goal_time_update = true;

			Mix_PlayChannel(-1, g_train_run_sound, 0);
		}
	}

	if (g_day == DAY_NIGHT && train_state_ == TRAIN_STOP) {
		g_current_game_phase = PHASE_ENDING;
		Mix_FreeMusic(g_bg_music);
		g_bg_music = Mix_LoadMUS("../../Resources/Ending_train.mp3");
		Mix_FadeInMusic(g_bg_music, -1, 2000);
		Mix_VolumeMusic(60);
	}
	train_destination_rect_.x += train_speed_;

	// Time값 Update
	PhaseInterface::TimeUpdate();
	if (g_time_sec % 10 == 0)
		g_time_update = true;
}

void Platform::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer);
	SDL_RenderCopy(g_renderer, train_texture_, &train_source_rect_, &train_destination_rect_);

	if (train2_count_ == 1)
	{
		SDL_RenderCopy(g_renderer, train2_texture_, &train2_source_rect_half_, &train2_destination_rect_);
	}
	else if (train2_count_ == 0)
	{
		SDL_RenderCopy(g_renderer, train2_texture_, &train2_source_rect_, &train2_destination_rect_);
	}

	SDL_RenderCopy(g_renderer, platform_texture_, &platform_source_rect_, &platform_destination_rect_);
	for (int i = 0; i < 10; i++) {
		door_destination_rect_.x = door_stop_destination_[i][0];
		door_destination_rect_.y = door_stop_destination_[i][1];
		if (i % 2 == 0) {
			SDL_RenderCopy(g_renderer, door_texture_, &door_source_rect_left_, &door_destination_rect_);
		}
		else {
			SDL_RenderCopy(g_renderer, door_texture_, &door_source_rect_right_, &door_destination_rect_);
		}
	}

	for (int i = 0; i < 30; i++) {
		if (people_destination_Array_[i][0] == 1 || people_destination_Array_[i][0] == 4) {
			people_destination_rect_.x = people_destination_Array_[i][1];
			people_destination_rect_.y = people_destination_Array_[i][2];
			SDL_RenderCopy(g_renderer, people_texture_, &people_source_rect_stop_, &people_destination_rect_);
		}
		else if (people_destination_Array_[i][0] == 3) {
			people_destination_rect_.x = people_destination_Array_[i][1];
			people_destination_rect_.y = people_destination_Array_[i][2];
			SDL_RenderCopy(g_renderer, people_texture_, &people_source_rect_right_, &people_destination_rect_);
		}
		else if (people_destination_Array_[i][0] == 2) {
			people_destination_rect_.x = people_destination_Array_[i][1];
			people_destination_rect_.y = people_destination_Array_[i][2];
			SDL_RenderCopy(g_renderer, people_texture_, &people_source_rect_left_, &people_destination_rect_);
		}

	}//
	if (sign_count_ == 1 && sign_time_ > 0)
	{
		sign_time_--;
		SDL_RenderCopy(g_renderer, sign_texture_, &sign_source_rect_good_, &sign_destination_rect_);
	}
	else if (sign_count_ == 2 && sign_time_ > 0)
	{
		sign_time_--;;
		SDL_RenderCopy(g_renderer, sign_texture_, &sign_source_rect_fail_, &sign_destination_rect_);
	}
	if (g_train_pos_update == true)
	{
		PhaseInterface::TrainPosUpdate();
		g_train_pos_update = false;
	}
	PhaseInterface::ShowUI();

	// 목표시간 Render
	if (g_goal_time_update)
	{
		PhaseInterface::SetGoalTimeFont();
		g_goal_time_update = false;
	}
	SDL_Rect tmp_r = { 145, 23, 110, 60 };
	SDL_Texture* tmp_texture = PhaseInterface::GetGoalTimeTexture();
	SDL_Rect tmp_rect = PhaseInterface::GetGoalTimeRect();
	SDL_RenderCopy(g_renderer, tmp_texture, &tmp_rect, &tmp_r);
	// Time Render
	if (g_time_update)
	{
		PhaseInterface::SetTimeFont();
		g_time_update = false;
	}
	SDL_Rect tmp_r2 = { 145, 92, 110, 60 };
	SDL_Texture* tmp_texture2 = PhaseInterface::GetTimeTexture();
	SDL_Rect tmp_rect2 = PhaseInterface::GetTimeRect();
	SDL_RenderCopy(g_renderer, tmp_texture2, &tmp_rect2, &tmp_r2);
	// Score
	if (g_score_update)
	{
		PhaseInterface::SetScoreFont();
		g_score_update = false;
	}
	SDL_Rect tmp_r3 = { 1070, 55, 70, 50 };
	SDL_Texture* tmp_texture3 = PhaseInterface::GetScoreTexture();
	SDL_Rect tmp_rect3 = PhaseInterface::GetScoreRect();
	SDL_RenderCopy(g_renderer, tmp_texture3, &tmp_rect3, &tmp_r3);

	if (train_state_ == TRAIN_IN) {
		PhaseInterface::FadeIn();
	}
	if (train_state_ == TRAIN_OUT) {
		PhaseInterface::FadeOut(3);
	}

	SDL_RenderPresent(g_renderer);

}

void Platform::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_UP) {
				// 위쪽 방향키를 누르면 기차가 다시 출발
				if (train_state_ == TRAIN_STOP && g_day != DAY_NIGHT && people_count == 0 && door_count_ == 87) {
					train2_count_ = 4;
					train_state_ = TRAIN_OUT;
					sign_count_ = 1;
					sign_time_ = 30;

					Mix_PlayChannel(-1, g_train_start_sound, 0);
				}
				else if (people_count != 0 && g_day != DAY_NIGHT) {
					sign_count_ = 2;
					sign_time_ = 30;
				}
				else if (g_day == DAY_NIGHT) {
					g_current_game_phase = PHASE_ENDING;
					g_time_update = true;
					g_score_update = true;
				}
			}
			break;
		}
	}
}

Platform::~Platform()
{
	SDL_DestroyTexture(platform_texture_);
	SDL_DestroyTexture(train_texture_);
	SDL_DestroyTexture(train2_texture_);
	SDL_DestroyTexture(door_texture_);
	SDL_DestroyTexture(people_texture_);
	SDL_DestroyTexture(sign_texture_);
}