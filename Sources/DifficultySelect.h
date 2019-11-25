#pragma once

#include "Scene.h"

enum DifficultyType
{
	DIFFICULTY_1,
	DIFFICULTY_2,
	DIFFICULTY_3,
	DIFFICULTY_4,
	DIFFICULTY_5,
	DIFFICULTY_6,
	DIFFICULTY_7,
};

class DifficultySelect : public BaseScene
{
public:
	int colorNum;

private:
	int difficultyNum;
	const int difficultyColorNum[7] = { 3, 4, 4, 5, 6, 6, 7 };

	bool isDifficulty1;
	bool isDifficulty2;
	bool isDifficulty3;
	bool isDifficulty4;
	bool isDifficulty5;
	bool isDifficulty6;
	bool isDifficulty7;

	int rightCnt;
	int leftCnt;

public:
	DifficultySelect() {}
	void Init();
	void Uninit();
	void Update();
	void Render();
};