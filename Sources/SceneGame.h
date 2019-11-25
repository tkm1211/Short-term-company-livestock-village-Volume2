#pragma once

#include "Scene.h"
#include <memory>
#include "Sprite.h"


class SceneGame : public BaseScene
{
public:
	int gameMode;
	int charNum[2];
	int gameOverDelayTime;
	int startTimer;

	bool isTwoPlayer;
	bool isGameOver_one;
	bool isGameOver_two;
	bool goTitle;		// タイトル
	bool goGameInit;	// リトライ
	bool isStartInit;	// れでぃ～ごーのやつ

public:
	SceneGame() {}
	void Init();
	void Uninit();
	void Update();
	void Render();
};

