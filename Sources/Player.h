#pragma once

#include <memory>
#include "sprite.h"

class Player
{
	static constexpr int MOVING_WIDTH = 114;
	static constexpr int MOVING_HEIGHT = 114;

	static constexpr int ACCEL_START = 15;
	static constexpr int ACCEL_MOVE_PER = 7;

	static constexpr int ADJUST = 3;
	static constexpr int SINGLE_CORRECTION_X = 615;
	static constexpr int SINGLE_CORRECTION_Y = 12;

private:
	std::shared_ptr<SpriteBatch> sprPickel;

	DirectX::XMFLOAT2 pos;
	int row;
	int column;
	int accelerationCount[8]; // ピッピピピピって感じで移動する時のカウント
	int animFrame;

	bool canMove;

public:
	Player() { Init(); }
	~Player() { Uninit(); }
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void OperatePlayer();
	void PositionCorreciton();
};

extern Player provisionalPlayer;