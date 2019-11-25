#pragma once

#include <DirectXMath.h>
#include <memory>
#include <sprite.h>

#include "Singleton.h"

//#define USE_IMGUI


class UI// : public Singleton<UI>
{
public:
	std::unique_ptr<SpriteBatch> sprUI;

	//DirectX::XMFLOAT2 meterPos;
	//DirectX::XMFLOAT2 blockPos[6];

	TexData meter;
	TexData meterBack;
	TexData speedUp;
	TexData arrow;
	TexData scoreBoard;
	TexData block[7];
	
	DirectX::XMFLOAT2 scorePos;

	int rowColor[6];

	float second;
	float maxSecond;
	int interpolationTimeCnt;

	int totalTime;
	int minTime;
	int secondTime;

	int totalScore;

	/*------òAçΩï`âÊån--------*/
	DirectX::XMFLOAT2 chainPos;
	float chainFirstPosY;
	int lastChainCount;
	int chainDisplayTimer;
	bool isDisplayChain;
	float chainDisplayAlpha;
	/*-----------------------*/

	/*----Ready Goån---------*/
	int readyTimer;
	float readyAlpha;
	int readyState;
	float readyPosY;
	float readyPosX;
	bool isDisplayGo;
	/*-----------------------*/

	bool isTimerStop;

	bool isSpeedUp;
	int textSpeedUpCnt;

	bool isGageMax;
	float alpha;
	int gageDownCnt;

	float tPosY;
	float tSizeY;
	float posY;

	float tryPosX;
	float tryPosY;

	float winLoseAlpha;
	bool  isWinLoseUp;

private:
	int speedUpCount;
	const int baseScore_tbl[6] = { 10,20,50,90,140,200 };

public:
	void Init(int);
	void Uninit();
	void Update(int);
	void Draw(int);

	int		calculateScore(int _eraseNum, int);
	void	SetChainDisplayInit(DirectX::XMFLOAT2 _pos);
	void	ChainDisplayUpdate();
	void	TwoPlayDrawChainCount(int);
	void	TwoPlayDrawScore(int);
};

//#define UI_INSTANCE  UI::getInstance()