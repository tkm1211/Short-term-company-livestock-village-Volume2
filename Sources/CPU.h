#pragma once

#include <memory>

#include "AI.h"
#include "Sprite.h"


class CPU
{
private:
	static constexpr int MOVING = 114;
	static constexpr int ACCEL_START = 15;
	static constexpr int ACCEL_MOVE_PER = 7;

private:
	// CPU AI
	std::unique_ptr<AI> cpuAI;

	// Sprite
	std::shared_ptr<SpriteBatch> sprPickel;

	// Sprite Data
	DirectX::XMFLOAT2 pos;
	int animFrame;

	// Input Data
	int row;
	int column;
	int accelerationCount[4]; // ピッピピピピって感じで移動する時のカウント

public:
	 CPU();
	~CPU();

public:
	void Init();
	void UnInit();
	void Update();
	void Draw();
	void OperateCPU();
	void PositionCorreciton();
	void SetBreakBlock();
};

extern CPU regularCPU;