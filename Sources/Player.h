#pragma once

#include <DirectXMath.h>
#include <memory>
#include <sprite.h>
#include <Audio.h>
#include "Singleton.h"

#define ACCEL_START		15
#define ACCEL_SPEED_PAR 7


class Player
{
public:
	static constexpr int MOVING_WIDTH = 114;
private:

	std::unique_ptr<SpriteBatch> sprPickel;
	std::unique_ptr<SpriteBatch> sprPlayer1[4];
	DirectX::XMFLOAT2 characterPos;

	int	accelerationCount[8];

	int animCount;
	int animFrame;

	TexData charTex;
	int charAnimCnt;
	int charAnimFrame;

	bool isPlaySoundBreak;
	bool isPlaySoundOjama;

	 int count;
	 int count2;
	 int count3;


public:
	DirectX::XMFLOAT2 pos;
	bool canMove;

	int row;
	int column;



public:
	void Init(int);
	void Uninit();
	void Update(int);
	void Draw(int);

	void Move(int);
	void Break(int);
	void Correction(int);
	void TwoPlayDraw(int);
};


//class PlayerManager : public Singleton<PlayerManager>
//{
//
//	enum PLAYER
//	{
//		ONE_P,
//		TWO_P,
//	};
//
//public:
//	Player player[2];
//	static constexpr int MOVING_WIDTH = 114;
//public:
//	void Init();
//	void Uninit();
//	void Update();
//	void Draw();
//};
//#define PLAYER_MANAGER PlayerManager::getInstance()