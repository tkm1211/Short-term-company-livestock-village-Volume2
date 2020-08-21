#pragma once

#include "Selector.h"
#include <memory>
#include "sprite.h"


class LevelSelector : public ISelector
{
private:
	const int levelMin = 1;
	const int levelMax = 7;
	const int levelDefault = 4;

	const int cpuPoworMin = 1;
	const int cpuPoworMax = 3;
	const int cpuPoworDefault = 1;

	const float levelPosX = 544.0f;
	const float levelSinglePosY = 1080.0f;
	const float levelMulti01PosY = 1080.0f;
	const float levelMulti02PosY = 1404.0f;
	const float levelCPUPosY = 1080.0f;
	const float levelSizeX = 840.0f;
	const float levelSizeY = 258.0f;
	const float levelTexPosY = 1534.0f;
	const float levelSelectedTexPosX = 0.0f;
	const float levelSelectedTexPosY = 2050.0f;

	const float cpuPoworPosX = 544.0f;
	const float cpuPoworPosY = 1080.0f + 258.0f - 78.0f; // 1662.0f
	const float cpuPoworSizeX = 606.0f;
	const float cpuPoworSizeY = 78.0f;
	const float cpuPoworTexPosX = 0.0f;
	const float cpuPoworTexPosY = 2794.0f;
	const float cpuPoworSelectedTexPosX = 0.0f;
	const float cpuPoworSelectedTexPosY = 2872.0f;

	const float characterLeftPosX = 246.0f;
	const float characterRightPosX = 1674.0f; // 1920.0f - 246.0f
	const float characterPosY = 436.0f;
	const float characterSizeX = 416.0f;
	const float characterSizeY = 416.0f;

	int moveCntMax = 60;
	float singleMoveSpeedY = 9.53333f;
	float multiMoveSpeedY = 12.5f;
	const float cpuCharacterMoveSpeedX = 11.0333f;

	const float stringStartPosX = 240.0f;
	const float stringStartPosY = -160.0f;
	const float stringEndPosX = 240.0f;
	const float stringEndPosY = 72.0f;
	const float stringMaxMove = 10.0f;
	const float stringSpeed = 0.05f;

	const int okMoveCntMax = 30;
	const int okAlphaCntMax = 15;
	const int okWaitCntMax = 20;
	const float okLeftPosX = 336.0f;
	const float okRightPosX = 1446.0f;
	const float okPosY = 508.0f;
	const float okGoalPosY = 366.0f;
	const float okSizeX = 138.0f;
	const float okSizeY = 72.0f;
	const float okTexPosX = 0.0f;
	const float okTexPosY = 1462.0f;
	const float okMoveSpeedY = 4.6666f;
	const float okAlphaSpeedY = 0.0666f;

	static constexpr int CHARACTER_ANIMATION_MAX = 6;
	static constexpr int ACCEL_START = 15;
	static constexpr int ACCEL_MOVE_PER = 7;

private:
	std::shared_ptr<SpriteBatch> sprSelect;
	std::shared_ptr<SpriteBatch> sprSelectCPU;
	std::shared_ptr<SpriteBatch> sprCharacters[8];

	DirectX::XMFLOAT2 pos[2];
	DirectX::XMFLOAT2 cpuPoworPos;
	float characterPosX[2];

	DirectX::XMFLOAT2 stringPos;
	float stringMoveY;
	float stringMoveSpeed;

	int controllerCnt;

	int levelNum[2];
	int charNum[2];
	bool decision[2];

	int cpuPoworNum;
	bool cpuPoworDecision;

	int moveCnt;

	bool operationLock;

	DirectX::XMFLOAT2 okPos[2];
	int okMoveCnt[2];
	float okAlpha[2];
	bool okAlphaFlg[2];
	bool okBegin[2];
	bool okEnd[2];

	DirectX::XMINT2 accelerationCount[2];

	bool changeCPUlevel;
	bool changeCPUPowor;
	bool changeCPUReturn;

public:
	int animCnt;
	int animFrame;

public:
	LevelSelector() {}
	~LevelSelector() {}

public:
	void Construct() override;
	void Init(ISelector* selector) override;
	bool Update() override;
	void Draw() override;

	int GetCharacterNum(int index) { return charNum[index]; }
	int GetLevel(int index) { return levelNum[index]; }
	int GetCPUPowor() { return cpuPoworNum - 1; }

private:
	void BeginMove();
	bool EndMove();
	void OkMove();
	void CPULevelMove();
	void CPUPoworMove();
	void Choice();
	void Operation();
};