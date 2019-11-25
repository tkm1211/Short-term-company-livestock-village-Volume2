#pragma once

#include "Scene.h"


enum CharacterType
{
	CHARACTER_1,
	CHARACTER_2,
	CHARACTER_3,
	CHARACTER_4,
};

class CharacterSelect : public BaseScene
{
public:
	int characterNum;

private:
	bool isCharacter1;
	bool isCharacter2;
	bool isCharacter3;
	bool isCharacter4;

	int rightCnt;
	int leftCnt;

public:
	CharacterSelect() {}
	void Init();
	void Uninit();
	void Update();
	void Render();
};