#pragma once

#include <DirectXMath.h>

class Block
{
private:
	static constexpr int FALL_SPEED = 114 / 6;//  =19 
	static constexpr int FALL_COUNT = 6;
	static constexpr int SIZE_X = 114;
	static constexpr int SIZE_Y = 114;

	DirectX::XMFLOAT2 pos;
	int color;		// �F
	int row;		// ���ԍ�
	int column;		// �c�ԍ�
	int moveCount;	// �ړ��\�ȕb��

	bool isExist;
	bool isFall;


public:
	Block() { Init(); }

	~Block() { Uninit(); }

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void OperateBlock();

public:
	void GenerateMe(int _row, int _column, int _color);

public:	// Getter and Setter
	bool	GetIsExist() { return isExist; }
	int		GetColumn() { return column; }
	int		GetRow() { return row; }
	int		GetSizeX() { return SIZE_X; }
	int		GetSizeY() { return SIZE_Y; }
	int		GetColor() { return color; }
	DirectX::XMFLOAT2 GetPos() { return pos; }

	void SetIsExist(bool _set) { isExist = _set; }
};
