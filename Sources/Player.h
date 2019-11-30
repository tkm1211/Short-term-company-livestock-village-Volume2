#pragma once

#include <memory>
#include "sprite.h"

class Player
{
	static constexpr int MOVING_WIDTH = 114;
	static constexpr int MOVING_HEIGHT = 114;
private:
	std::shared_ptr<SpriteBatch> sprPickel;

	DirectX::XMFLOAT3 pos;
	int row;
	int column;

	bool canMove;

public:
	Player() { Init(); }
	~Player() { Uninit(); }
	void Init();
	void Uninit();
	void Update();
	void Draw();
};