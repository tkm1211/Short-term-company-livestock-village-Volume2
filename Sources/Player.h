#pragma once

#include <memory>
#include <array>
#include "sprite.h"

class Player
{
	static constexpr int MOVING_WIDTH = 114;
	static constexpr int MOVING_HEIGHT = 114;

	static constexpr int ACCEL_START = 15;
	static constexpr int ACCEL_MOVE_PER = 7;

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
	void Update(int _pn = 0);
	void Draw();
	void DrawOfSingle();
	void DrawOfMulti(int _pn);

	void OperatePlayer(int _pn);
	void SetBreakBlock(int _pn);

	void PositionCorreciton();
};

extern Player provisionalPlayer;
extern std::array<Player, 2> regularPlayer;