#pragma once

#include "Core.h"

enum EventsName
{
	EVENT_NONE,
	EVENT_HEAT,
	EVENT_COLD,
	EVENT_MERCHANT,
	EVENT_PSEUDO,
	EVENT_EMERGENCY,
	EVENT_WILD,
	EVENT_DRUNK
};

enum CommandCond
{
	COMMAND_FAIL, COMMAND_PASS, COMMAND_NONE
};

class Events
{
private:
	SDL_Texture* events_texture_;
	SDL_Rect events_source_rect_[8];
	SDL_Rect events_destination_rect_[8];

	SDL_Texture* command_texture_;
	SDL_Texture* command_texture_green_;
	SDL_Rect command_source_rect_[4];
	SDL_Rect command_destination_rect_[5];

	SDL_Texture* output_texture_[3];
	SDL_Rect output_rect_;
	TTF_Font* output_font_;
	SDL_Color black_;

	SDL_Color red_;
	SDL_Color green_;

	int distance_; // Runnig���� ������ distance�� ����
	bool eventState_;// �̺�Ʈ �߻������� �ƴ���
	int commandState_;// Ŀ�ǵ� ��������
	int command_[5];
	int trueCommand_[5];
	int commandCount_;
	int random_;// �̺�Ʈ ���� ������
	int passCount_;// Ŀ�ǵ� ���� Ƚ��( Ŀ�ǵ带 ���� ��Ʈ �����ؾ� ��� ��� ���ؼ�)

	bool time_out_;
	bool right_key_;
	int past_;


public:
	Events();
	~Events();

	int getPassOrFail();
	bool getEventState();
	void eventSet();
	void runEvent(int dis);
	void showEvent();
	void commandHandle();
	void compareCommand();
	void SetTimeOut(bool);
	void SetPast(int i);
	int GetPast();

	void SetRightKey(bool i);
	bool GetRightKey();

private:
	void handleKeyDown(SDL_Keycode key);
	void resetEvent();
};

class EventScore 
{
private:
	bool one_;

public:
	EventScore();
	~EventScore();

	void SetOne(bool i);
	bool GetOne();


};