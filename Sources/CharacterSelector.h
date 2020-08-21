#pragma once

#include "Selector.h"
#include <memory>
#include <sprite.h>


class CharacterSelector : public ISelector
{
private:
	const int characterMin = 1;
	const int characterMax = 7;
	const int player01CharacterDefault = 0;
	const int player02CharacterDefault = 1;

	const float characterLeftPosX = -416.0f;
	const float characterRightPosX = 2336.0f;
	const float characterPosY = 436.0f;
	const float characterSizeX = 416.0f;
	const float characterSizeY = 416.0f;
	const float characterFrameOriginPosX = 660.0f;
	const float characterFrameOriginPosY = 508.0f;
	const float characterFrameStartOriginPosY = 1080.0f;
	const float characterFrameSizeX = 150.0f;
	const float characterFrameSizeY = 150.0f;
	const float characterFrameTexPosX = 0.0f;
	const float characterFrameTexPosY = 1012.0f;
	const float characterPickTexPosX = 0.0f;
	const float characterPickTexPosY = 1162.0f;
	const float characterPickSizeX = 150.0f;
	const float characterPickSizeY = 150.0f;

	const float stringStartPosX = 240.0f;
	const float stringStartPosY = -160.0f;
	const float stringEndPosX = 240.0f;
	const float stringEndPosY = 72.0f;
	const float stringMaxMove = 10.0f;
	const float stringSpeed = 0.05f;

	const int moveCntMax = 60;
	const float moveSpeedX = 11.0333f;
	const float moveSpeedY = 9.53333f;

	const int okMoveCntMax = 30;
	const int okAlphaCntMax = 15;
	const int okWaitCntMax = 20;
	const float okLeftPosX = 336.0f;
	const float okRightPosX = 1446.0f;
	const float okPosY = 515.0f;
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

	DirectX::XMFLOAT2 characterPos[2];
	DirectX::XMFLOAT2 characterSize[2];
	DirectX::XMFLOAT2 characterFrameOriginPos;
	DirectX::XMFLOAT2 characterFrameSize;
	DirectX::XMFLOAT2 characterPick[2];

	DirectX::XMFLOAT2 stringPos;
	float stringMoveY;
	float stringMoveSpeed;

	bool endMoveReturn;
	bool operationLock;

	int selectCharacterNum[2];
	bool characterDecision[2];
	int selectCPUCharacterState;

	int moveCnt;

	bool cpuCharacterMove;
	bool cpuCharacterMoveReturn;
	int cpuCharacterMoveCnt;

	DirectX::XMFLOAT2 okPos[2];
	int okMoveCnt[2];
	float okAlpha[2];
	bool okAlphaFlg[2];
	bool okBegin[2];
	bool okEnd[2];

	SelectMode previousSelectorMode;

	DirectX::XMINT2 accelerationCount[2];

public:
	int animCnt;
	int animFrame;

public:
	CharacterSelector() {}
	~CharacterSelector() {}

public:
	void Construct() override;
	void Init(ISelector* selector) override;
	bool Update() override;
	void Draw() override;

	int GetSelectCharacterNum(int index) { return selectCharacterNum[index]; }

private:
	void BeginMove();
	bool EndMove();
	void CPUMove();
	void OkMove();
	void Choice();
	void Operation();
};