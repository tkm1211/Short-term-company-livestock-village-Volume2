#pragma once

#include <Audio.h>
#include "Singleton.h"

class Sound : public Singleton<Sound>
{
public:
	enum SE
	{
		BREAK,
		MOVE,
		DESTRUCTION1,
		GAUGE_MAX,
		LEVEL_SELECT,
		OK,
		RESULT,
		SCENE_CHANGE,
		SPEED_UP,
		GO,
		OJAMA,
		READY,
		ALEAT,
		GAME_END,
		LOGO,
		LOGO_CAN,
		SE_END,
	};
	enum BGM
	{
		TITLE,
		SELECT,
		GAME,
		BGM_END,
	};

	AudioHandle* seHandle[SE::SE_END];
	AudioHandle* bgmHandle[BGM::BGM_END];

public:
	void Load();
	void Update();
	void Uninit();
};

#define SOUND Sound::getInstance()