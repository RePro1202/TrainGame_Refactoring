#include "GameFunc_Running.h"

Running::Running()
{
	// Background
	SDL_Surface* background1_surface = IMG_Load("../../Resources/Background_morning.png");
	background_texture_[0] = SDL_CreateTextureFromSurface(g_renderer, background1_surface);
	SDL_FreeSurface(background1_surface);
	background_source_rect_[0] = { 0, 0 ,2400 ,485 };
	background_destination_rect_[0] = { 0, 0, background_source_rect_[0].w, background_source_rect_[0].h };

	SDL_Surface* background2_surface = IMG_Load("../../Resources/Background_daytime.png");
	background_texture_[1] = SDL_CreateTextureFromSurface(g_renderer, background2_surface);
	SDL_FreeSurface(background2_surface);
	background_source_rect_[1] = { 0, 0 ,2400 ,485 };
	background_destination_rect_[1] = { 0, 0, background_source_rect_[1].w, background_source_rect_[1].h };

	SDL_Surface* background3_surface = IMG_Load("../../Resources/Background_evening.png");
	background_texture_[2] = SDL_CreateTextureFromSurface(g_renderer, background3_surface);
	SDL_FreeSurface(background3_surface);
	background_source_rect_[2] = { 0, 0 ,2400 ,485 };
	background_destination_rect_[2] = { 0, 0, background_source_rect_[2].w, background_source_rect_[2].h };
	// Track
	SDL_Surface* track_surface = IMG_Load("../../Resources/Track.png");
	track_texture_ = SDL_CreateTextureFromSurface(g_renderer, track_surface);
	SDL_FreeSurface(track_surface);
	track_source_rect_ = { 0, 0 ,2400 ,318 };
	track_destination_rect_ = { 0, 485, track_source_rect_.w, track_source_rect_.h };
	// Train
	SDL_Surface* train_surface = IMG_Load("../../Resources/Train.png");
	SDL_SetColorKey(train_surface, SDL_TRUE, SDL_MapRGB(train_surface->format, 100, 170, 150));
	train_texture_ = SDL_CreateTextureFromSurface(g_renderer, train_surface);
	SDL_FreeSurface(train_surface);
	train_source_rect_ = { 93, 57 ,1753 ,184 };
	train_destination_rect_ = { -1188, 530, train_source_rect_.w, train_source_rect_.h };
	// Arrow
	SDL_Surface* arrow_surface = IMG_Load("../../Resources/UI.png");
	SDL_SetColorKey(arrow_surface, SDL_TRUE, SDL_MapRGB(arrow_surface->format, 200, 200, 250));
	arrow_texture_ = SDL_CreateTextureFromSurface(g_renderer, arrow_surface);
	SDL_FreeSurface(arrow_surface);
	arrow_source_rect_ = { 697, 76 ,12 ,35 };
	arrow_destination_rect_ = { 1080, 186, arrow_source_rect_.w, arrow_source_rect_.h };

	train_speed_ = 54;
	train_distance_ = 0;
	arrow_speed_ = 1.3;

	RightKey = false;

	eve_ = new Events;
	e_score = new EventScore;
}

void Running::Update()
{
	eve_->runEvent(train_distance_);	// distance값으로 이벤트 발생조건 검사

	if (!eve_->getPassOrFail() && train_distance_ < 5) {	// 시간초과 커맨드 실패 판단
		eve_->SetPast(0);
	}
	if (train_distance_ >= 5 && eve_->GetPast() != 0) {
		eve_->SetTimeOut(true);
	}

	if (e_score->GetOne() && (eve_->getPassOrFail() == 1))	// 이벤트 점수 부여
	{
		PhaseInterface::IncreaseScore(500);
		e_score->SetOne(false);
		g_score_update = true;
	}
	else if (e_score->GetOne() && (eve_->getPassOrFail() == 0)) {
		PhaseInterface::DecreaseScore(500);
		e_score->SetOne(false);
		g_score_update = true;
	}

	// 이벤트 처리하는 동안 속도, 속도계 고정
	if (!eve_->getEventState()) {
		RightKey = eve_->GetRightKey();

		if (RightKey) {
			train_speed_ += 2.5;
			arrow_speed_ = -3.0;
		}
		else if (!RightKey) {
			train_speed_ -= 1;	// speed 자동 감소(최솟값 5, 최댓값 60)
			if (train_speed_ < 10)
				train_speed_ = 10;
			else if (train_speed_ > 54)
				train_speed_ = 54;
		}

		arrow_destination_rect_.x -= arrow_speed_;

		if (arrow_destination_rect_.x > 1064) //속도계상에서의 열차 속도 제한
			train_speed_ = 54;
	}

	// 위쪽 배경
	background_destination_rect_[g_day].x -= train_speed_ / 2;

	if (background_destination_rect_[g_day].x < -1200) {
		background_destination_rect_[g_day].x = 0;
		++train_distance_;
	}
	// 아래쪽 배경
	track_destination_rect_.x -= train_speed_ * 2;

	if (track_destination_rect_.x < -1200) {

		track_destination_rect_.x = 0;
	}

	// 속도계 화살표
	if (arrow_destination_rect_.x < 954) {
		arrow_destination_rect_.x = 954;
	}
	else if (arrow_destination_rect_.x > 1131) {
		arrow_destination_rect_.x = 1131;
	}
	if (!eve_->getEventState()) {	// 이벤트중 점수 변화 멈춤
		//속도계 눈금 벗어났을때 Score감소
		if (arrow_destination_rect_.x < 1064 || arrow_destination_rect_.x > 1096) {
			PhaseInterface::DecreaseScore(10);
			g_score_update = true;
		}
		else
		{
			PhaseInterface::IncreaseScore(3);
			g_score_update = true;
		}
	}

	arrow_speed_ = 1.3;


	// 페이즈 전환
	if (train_distance_ == 10)
	{
		background_destination_rect_[g_day].x = 0;
		track_destination_rect_.x = 0;

		train_destination_rect_.x += 40;
		if (train_destination_rect_.x > BLOCK_X_MAX)
		{
			g_current_game_phase = PHASE_PLATFORM;
			PhaseInterface::EndFade();
			e_score->SetOne(true);
			eve_->SetPast(1);
			eve_->SetTimeOut(false);

			train_speed_ = 54;
			train_distance_ = 0;
			train_destination_rect_.x = -1188;
			arrow_destination_rect_.x = 1080;
			Mix_PauseMusic();

			g_time_update = true;
			g_score_update = true;
			g_train_pos_update = true;
			g_goal_time_update = true;
			++g_day;// 시간대 변경

			RightKey = false;
			eve_->SetRightKey(false);
		}
	}

	// Time값 Update
	PhaseInterface::TimeUpdate();
	if (g_time_sec % 10 == 0)
		g_time_update = true;
}

void Running::Render()
{
	// Background
	SDL_RenderCopy(g_renderer, background_texture_[g_day], &background_source_rect_[g_day], &background_destination_rect_[g_day]);
	SDL_RenderCopy(g_renderer, track_texture_, &track_source_rect_, &track_destination_rect_);

	// Train
	SDL_RenderCopy(g_renderer, train_texture_, &train_source_rect_, &train_destination_rect_);

	if (g_train_pos_update == true)
	{
		PhaseInterface::TrainPosUpdate();
		g_train_pos_update = false;
	}
	PhaseInterface::ShowUI();
	eve_->showEvent();

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

	//arrow
	SDL_RenderCopy(g_renderer, arrow_texture_, &arrow_source_rect_, &arrow_destination_rect_);

	// Fade in and out
	if (train_distance_ == 0) {
		PhaseInterface::FadeIn();
	}
	else if (train_distance_ == 2) {
		PhaseInterface::EndFade();
	}
	else if (train_distance_ == 10) {
		PhaseInterface::FadeOut(5);
	}

	SDL_RenderPresent(g_renderer);
}

void Running::HandleEvents()
{
	SDL_Event event;

	eve_->commandHandel();
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_RIGHT)
			{	// 오른쪽 키를 누르면 speed값 증가
				RightKey = true;
				eve_->SetRightKey(true);
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_RIGHT) {
				RightKey = false;
				eve_->SetRightKey(false);
			}
			break;
		}
	}
}

Running::~Running()
{
	SDL_DestroyTexture(background_texture_[0]);
	SDL_DestroyTexture(background_texture_[1]);
	SDL_DestroyTexture(background_texture_[2]);
	SDL_DestroyTexture(track_texture_);
	SDL_DestroyTexture(train_texture_);
	SDL_DestroyTexture(arrow_texture_);
}