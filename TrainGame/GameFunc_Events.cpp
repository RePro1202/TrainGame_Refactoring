#include "GameFunc_Events.h"
#include <cstdlib>

#include "TSDL.h"

Events::Events()
{
	events_texture_ = TSDL::CreateTexture("../../Resources/Event.png", Color(112, 146, 190));
	command_texture_ = TSDL::CreateTexture("../../Resources/wasd.png", Color(255, 255, 255));
	command_texture_green_ = TSDL::CreateTexture("../../Resources/wasd.png", Color(255, 255, 255));
	SDL_SetTextureColorMod(command_texture_green_, 0, 255, 0);


	// 성공 실패 메세지 폰트
	output_font_ = TTF_OpenFont("../../Resources/Robotomono.ttf", 80);
	black_ = { 0,0,0,0 };
	red_ = { 170,0,0,180 };
	green_ = { 0,170,0,180 };

	output_texture_[0] = TSDL::CreateTextureToFont(output_font_, "Success!", green_, output_rect_);
	output_texture_[1] = TSDL::CreateTextureToFont(output_font_, "  Fail  ", red_);
	output_texture_[2] = TSDL::CreateTextureToFont(output_font_, "Time out", red_);
	
	TTF_CloseFont(output_font_);


	// 이벤트 이미지 src_rec
	events_source_rect_[EVENT_NONE] = { 800, 397 , 250 , 219 };		// Event Box
	events_source_rect_[EVENT_HEAT] = { 27, 8 ,247 , 300 };			// 더위
	events_source_rect_[EVENT_COLD] = { 290, 10 , 255 , 300 };		// 추위
	events_source_rect_[EVENT_MERCHANT] = { 608, 30 , 186 , 285 };	// 잡상인
	events_source_rect_[EVENT_PSEUDO] = { 850, 51 , 138 , 229 };	// 사이비
	events_source_rect_[EVENT_EMERGENCY] = { 76, 435 , 148 , 146 };	// 긴급정지
	events_source_rect_[EVENT_WILD] = { 286, 437 , 180 , 153 };		// 야생동물
	events_source_rect_[EVENT_DRUNK] = { 505, 422 , 215 , 196 };	// 취객

	// 이벤트 이미지 des_rec
	events_destination_rect_[EVENT_NONE] = { 210, 310 , 230 , 220 };
	for (int i = 1; i < 8; i++) {
		events_destination_rect_[i] = { 220, 320, 200, 180 };
	}

	// 커맨드 소스
	command_source_rect_[0] = { 0, 0, 270, 270 };
	command_source_rect_[1] = { 0, 270, 270, 270 };
	command_source_rect_[2] = { 270, 270, 270, 270 };
	command_source_rect_[3] = { 270, 0, 270, 270 };

	// 커맨드 위치
	int x = 600;
	for (int i = 0; i < 5; i++) {
		command_destination_rect_[i] = { x, 300, 100, 100 };
		x += 100;
	}

	distance_ = 0;	// Runnig에서 가져온 distance값 저장
	eventState_ = false;	// 이벤트 발생중인지 아닌지
	random_ = rand() % 7 + 1;	// 이벤트 종류 랜덤값
	commandState_ = COMMAND_NONE;	// 커맨드 성공여부
	commandCount_ = 0;	// 커맨드 카운트
	passCount_ = 0;	// 커맨드 성공 횟수( 커맨드를 여러 세트 성공해야 통과 기능 위해서)
	past_ = COMMAND_NONE;

	for (int i = 0; i < 5; i++) {
		command_[i] = 0;
		trueCommand_[i] = rand() % 4;	// 정답 커맨드 랜덤 생성
	}

	time_out_ = false;
	right_key_ = false;
}

Events::~Events()
{
	SDL_DestroyTexture(events_texture_);
	SDL_DestroyTexture(command_texture_);
	SDL_DestroyTexture(command_texture_green_);
	SDL_DestroyTexture(output_texture_[0]);
	SDL_DestroyTexture(output_texture_[1]);
	SDL_DestroyTexture(output_texture_[2]);
}

void Events::runEvent(int dis) {
	distance_ = dis;

	if (distance_ >= 2 && distance_ < 6) {
		eventSet();
		if (distance_ >= 5) {
			commandState_ = COMMAND_FAIL;
		}
	}
	else {
		commandState_ = COMMAND_NONE;
		passCount_ = 0;
		eventState_ = false;
		random_ = rand() % 7 + 1;
		commandCount_ = 0;


		for (int i = 0; i < 5; i++) {
			trueCommand_[i] = rand() % 4;
		}
	}
};

void Events::eventSet() {

	switch (commandState_)
	{
	case COMMAND_NONE:
		eventState_ = true;
		break;
	case COMMAND_PASS:
		if (passCount_ == 2) {
			eventState_ = false;

		} break;
	case COMMAND_FAIL:
		eventState_ = false;
		break;
	default:
		eventState_ = false;
		random_ = rand() % 7 + 1;
		break;
	}
}

void Events::showEvent() {

	if (eventState_) {
		// 이벤트 종류 출력
		SDL_RenderCopy(g_renderer, events_texture_, &events_source_rect_[0], &events_destination_rect_[0]);
		SDL_RenderCopy(g_renderer, events_texture_, &events_source_rect_[random_], &events_destination_rect_[random_]);

		// 커맨드 출력
		for (int i = 0; i < 5; i++) {
			SDL_RenderCopy(g_renderer, command_texture_, &command_source_rect_[trueCommand_[i]], &command_destination_rect_[i]);
			for (int j = 0; j < commandCount_; j++) {	// 커맨드 성공시 초록색으로 바꿈
				SDL_RenderCopy(g_renderer, command_texture_green_, &command_source_rect_[trueCommand_[j]], &command_destination_rect_[j]);
			}
		}
	}

	SDL_Rect tmp_r;
	tmp_r = { 400, 200, output_rect_.w, output_rect_.h };

	if (passCount_ == 2) {
		SDL_RenderCopy(g_renderer, output_texture_[0], &output_rect_, &tmp_r);
	}
	else if (commandState_ == COMMAND_FAIL && !time_out_) {
		SDL_RenderCopy(g_renderer, output_texture_[1], &output_rect_, &tmp_r);
	}
	else if (time_out_ && distance_ < 6) {
		SDL_RenderCopy(g_renderer, output_texture_[2], &output_rect_, &tmp_r);
	}
}

void Events::compareCommand() {

	if (command_[commandCount_] == trueCommand_[commandCount_])
	{
		if (commandCount_ == 4) {
			commandState_ = COMMAND_PASS;
			commandCount_ = 0;
			passCount_++;
			for (int i = 0; i < 5; i++) {
				trueCommand_[i] = rand() % 4;
			}
		}
		else {
			commandCount_++;
		}
	}
	else {
		commandState_ = COMMAND_FAIL;
		commandCount_ = 0;
	}
}

void Events::commandHandel() {

	SDL_Event event;

	if (eventState_ && SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (commandCount_ < 5) {
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					command_[commandCount_] = 0;
					compareCommand(); break;
				case SDLK_s:
					command_[commandCount_] = 2;
					compareCommand(); break;
				case SDLK_a:
					command_[commandCount_] = 1;
					compareCommand(); break;
				case SDLK_d:
					command_[commandCount_] = 3;
					compareCommand(); break;
				default: break;
				}
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				right_key_ = true;
			}
			break;

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_RIGHT) {
				right_key_ = false;
			}
		default:
			break;
		}
	}
}

void Events::SetTimeOut(bool i) {
	time_out_ = i;
}

void Events::SetPast(int i) {
	past_ = i;
}

int Events::GetPast() {
	return past_;
}

int Events::getPassOrFail() {
	// fail = 0, pass = 1, none = 2
	if (passCount_ == 2) {
		return 1;
	}
	else if (commandState_ == COMMAND_FAIL) {
		return 0;
	}
	else
		return 2;
}

bool Events::getEventState() {
	return eventState_;
}

// 오른쪽 방향키관련 
void Events::SetRightKey(bool i) {
	right_key_ = i;
}

bool Events::GetRightKey() {
	return right_key_;
}

//===============================================================

EventScore::EventScore() {
	one_ = true;

}

EventScore::~EventScore() {

}

void EventScore::SetOne(bool i) {
	one_ = i;
}

bool EventScore::GetOne() {
	return one_;
}