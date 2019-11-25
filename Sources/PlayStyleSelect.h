#pragma once

#include "Scene.h"

#include <memory>
#include <sprite.h>


enum PlayStyle
{
	ONE_PLAYER,
	TWO_PLAYER
};

class PlayStyleSelect : public BaseScene
{
public:
	int playStyle;

private:
	const float CHAR2_POS_Y = 1920.0f - (1920.0f - 246.0f - 319.0f);

	std::unique_ptr<SpriteBatch> sprSelectUI;
	std::unique_ptr<SpriteBatch> sprChar[2];

	TexData logo;
	TexData text;
	TexData onePlayMode;
	TexData twoPlayMode;
	TexData char1;
	TexData char2;

	bool isOnePlayer;
	bool isTwoPlayer;

	bool isDecision;
	int cnt;
	int sleep;

public:
	PlayStyleSelect() {}
	void Init();
	void Uninit();
	void Update();
	void Render();
};