#pragma once
#include <DirectXMath.h>


class Block
{
public:
	static constexpr int SPEED = 19;
	static constexpr int SIZE_X = 114;
	static constexpr int SIZE_Y = 114;

	DirectX::XMFLOAT2 pos;
	int		color;		// 色
	int		column;		// 縦番号
	int		row;		// 横

	int		elementNum;	// 要素数
	int		moveCount;
	int		eraseNum;	// 削除するときにいる

	bool isEnable;
	bool isFall;
	bool isNextFall;
	bool isErase;		// 連鎖して消える系のやつ

public:
	Block()
	{
		pos = DirectX::XMFLOAT2(0.0f, 0.0f);
		color = -1;		// 色
		column = -1;		// 縦番号
		row = -1;		// 横

		elementNum = -1;	// 要素数
		moveCount = -1;
		eraseNum = -1;	// 削除するときにいる

		isEnable = false;
		isFall = false;
		isNextFall = false;
		isErase = false;		// 連鎖して消える系のやつ
	}
	~Block()
	{
		pos = DirectX::XMFLOAT2(0.0f, 0.0f);
		color = -1;		// 色
		column = -1;		// 縦番号
		row = -1;		// 横

		elementNum = -1;	// 要素数
		moveCount = -1;
		eraseNum = -1;	// 削除するときにいる

		isEnable = false;
		isFall = false;
		isNextFall = false;
	}
	void Init();
	void Uninit();
	void Update(int);
	void Draw(int);

	void Move();
	void NextMove(int);

	void chackDownBlock(int);
};
