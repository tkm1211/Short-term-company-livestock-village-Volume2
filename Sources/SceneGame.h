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
	bool goTitle;		// �^�C�g��
	bool goGameInit;	// ���g���C
	bool isStartInit;	// ��ł��`���[�̂��

public:
	SceneGame() {}
	void Init();
	void Uninit();
	void Update();
	void Render();
};

