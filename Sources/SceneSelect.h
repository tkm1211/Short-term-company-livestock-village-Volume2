#pragma once

#include "Scene.h"

#include <memory>
#include <sprite.h>

enum SelectState
{
	MODE_SELECT,
	CHAR_SELECT,
	LEVEL_SELECT
};

enum GameMode
{
	ONE_PLAYER,
	TWO_PLAYER
};

enum CharacterType
{
	CHARACTER_1,
	CHARACTER_2,
	CHARACTER_3,
	CHARACTER_4,
};

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


class SceneSelect : public BaseScene
{
private: // common
	int selectState;
	int lastSelectState;

	std::unique_ptr<SpriteBatch> sprSelectUI;
	std::unique_ptr<SpriteBatch> sprChar[4];
	std::unique_ptr<SpriteBatch> sprBackBG;

	TexData logo;
	TexData text1;

	TexData char1; // プレイヤー１キャラクター
	TexData char2; // プレイヤー２キャラクター

	TexData ok1;
	TexData ok2;

	bool isDecision;
	bool isNextMove;

	int charAnimCnt;
	int charAnimFrame;

public: // mode
	int gameMode;

private: // mode
	const float CHAR2_POS_X = 1920.0f - (1920.0f - 246.0f - 319.0f);
	const float POS_Y = 658.0f;

	TexData onePlayMode;
	TexData twoPlayMode;

	bool isOnePlayer;
	bool isTwoPlayer;

	float cnt;
	int sleep;

	float alpha;

	bool isPlaySound;

public: // char
	int characterNum;
	int characterNum2;

private: // char
	TexData charList;
	TexData cursol1;
	TexData cursol2;

	bool isMove;

	bool isCharacter1;
	bool isCharacter2;
	bool isCharacter3;
	bool isCharacter4;

	bool isCharacter1_2;
	bool isCharacter2_2;
	bool isCharacter3_2;
	bool isCharacter4_2;

	int rightCnt;
	int leftCnt;

	int rightCnt2;
	int leftCnt2;

	bool isDecisionOnePlayer;
	bool isDecisionTwoPlayer;

	bool isOkOnePlayer;
	bool isOkTwoPlayer;

	int twoPlayerCnt;

	int onePlayerCnt_B;
	int twoPlayerCnt_B;

	float moveCnt1;
	float moveCnt2;

public: // level
	int colorNum;
	int colorNum2;

	float secondNum;
	float secondNum2;

private: // level
	TexData levelList1;
	TexData levelList2;

	TexData levelListFilter1;
	TexData levelListFilter2;

	int difficultyNum;
	int difficultyNum2;
	const int difficultyColorNum[7] = { 4, 5, 5, 6, 6, 7, 7 };
	//TODO : 数値を決めてください
	const float dificultyGaugeSpeedNum[7] = { 6.5f, 6.5f, 6.0f ,6.5f ,6.0f ,6.5f ,6.0f };
	bool isDecisionOnePlayer2;
	bool isDecisionTwoPlayer2;

	bool isOkOnePlayer2;
	bool isOkTwoPlayer2;

	int rightCnt_2;
	int leftCnt_2;

	int rightCnt2_2;
	int leftCnt2_2;

public:
	SceneSelect() {}
	void Init();
	void Uninit();
	void Update();
	void Render();

private:
	void  ModeSelectInit();
	void  CharSelectInit();
	void levelSelectInit();
	void  ModeSelectUpdate();
	void  CharSelectUpdate();
	void levelSelectUpdate();
	void  ModeSelectDraw();
	void  CharSelectDraw();
	void levelSelectDraw();

	void OnePlayerUpdate();
	void TwoPlayerUpdate();

	void OnePlayerLevelUpdate();
	void TwoPlayerLevelUpdate();

	void LevelSelectUnInit();
};