#pragma once

#include <memory>
#include <array>
#include <Audio.h>

class Sound
{
public:
	static constexpr int GAME_BGM_MAX = 3;

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
		LANDING_SMALL_OBSTACLE,
		LANDING_LARGE_OBSTACLE,
		BACK,
		SE_END,
	};
	enum BGM
	{
		TITLE,
		SELECT,
		GAME,
		GAME2,
		GAME3,
		BGM_END,
	};

	//AudioHandle* seHandle[SE::END];
	//AudioHandle* bgmHandle[BGM::END];
	std::array<std::unique_ptr<AudioHandle>, SE::SE_END> seHandle;
	std::array<std::unique_ptr<AudioHandle>, BGM::BGM_END> bgmHandle;

public:
	void Load();
	void Update();
	void Uninit();


	// SingletonŠÖŒW
private:
	Sound() = default;
	~Sound() = default;
	static Sound* instance;

public:
	static Sound* Get()
	{
		return instance;
	}
	static void Create()
	{
		if (!instance)
		{
			instance = new Sound();
		}
	}
	static void Destroy()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}
};
