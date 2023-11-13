
#include <iostream>
#include "SDL.h"
#include "Core.h"
#include "GameFunc_Intro.h"
#include "GameFunc_Running.h"
#include "GameFunc_Platform.h"
#include "GameFunc_Ending.h"

// 기본 창
SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_flag_running;
Uint32 g_last_time_ms;

// 페이즈 전환변수
int g_current_game_phase;

int g_day;
int g_time_hour;
int g_time_min;
int g_time_sec;
int g_train_pos;
int g_score;
bool g_goal_time_update;
bool g_time_update;
bool g_score_update;
bool g_train_pos_update;
Mix_Music* g_bg_music;
Mix_Chunk* g_train_run_sound;
Mix_Chunk* g_train_start_sound;

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}

	g_window = SDL_CreateWindow("First Window", 100, 30, 1200, 800, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);

	InitGame();
	PhaseInterface* game_phase[4];
	game_phase[PHASE_INTRO] = new Intro;
	game_phase[PHASE_PLATFORM] = new Platform;
	game_phase[PHASE_RUNNING] = new Running;
	game_phase[PHASE_ENDING] = new Ending;

	g_last_time_ms = SDL_GetTicks();

	while (g_flag_running)
	{
		Uint32 cur_time_ms = SDL_GetTicks();
		if (cur_time_ms - g_last_time_ms < 33)
			continue;

		game_phase[g_current_game_phase]->HandleEvents();
		game_phase[g_current_game_phase]->Update();
		game_phase[g_current_game_phase]->Render();

		g_last_time_ms = cur_time_ms;
	}

	for (int i = 0; i < 4; ++i)
	{
		delete game_phase[i];
	}

	ClearGame();
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	Mix_CloseAudio();
	SDL_Quit();

	return 0;
}