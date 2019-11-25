#pragma once

#include <sprite.h>
#include <memory>

enum PauseSelectState
{
	RETURN_GAME,
	RETRY,
	TO_TITLE,
};

class Pause
{
public:
	bool isOn;

private:
	std::unique_ptr<SpriteBatch> sprPause;
	std::unique_ptr<SpriteBatch> sprPauseBack;

	TexData pauseBack;
	TexData back;
	TexData select;

	int selectState;
	int pushCnt[2];

public:
	Pause() {}
	void Init();
	void Uninit();
	void Update();
	void Render();
};

extern Pause pause;