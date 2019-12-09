#include "BlockManager.h"
#include "Resource.h"
/*--------------------------------------*/
//	Global area
/*--------------------------------------*/

// unknown

/*--------------------------------------*/
//	Menber function
/*--------------------------------------*/

void BlockManager::Init()
{
	sprBlock = RESOURCE->GetSpriteData(Resource::Texture::Block);

	for (auto& it : block)
	{
		it.Init();
	}


	GenerateBlock(0, 5, Color::Green);
	GenerateBlock(1, 5, Color::Yellow);
	GenerateBlock(2, 5, Color::LightBlue);
	GenerateBlock(3, 5, Color::Red);
	GenerateBlock(4, 5, Color::Green);
	GenerateBlock(5, 5, Color::Yellow);

	GenerateBlock(0, 6, Color::Green);
	GenerateBlock(1, 6, Color::Yellow);
	GenerateBlock(2, 6, Color::LightBlue);
	GenerateBlock(3, 6, Color::Red);
	GenerateBlock(4, 6, Color::Green);
	GenerateBlock(5, 6, Color::Yellow);

	GenerateBlock(0, 7, Color::Green);
	GenerateBlock(1, 7, Color::Yellow);
	GenerateBlock(2, 7, Color::LightBlue);
	GenerateBlock(3, 7, Color::Red);
	GenerateBlock(4, 7, Color::Green);
	GenerateBlock(5, 7, Color::Yellow);

	GenerateBlock(0, 8, Color::Green);
	GenerateBlock(1, 8, Color::Yellow);
	GenerateBlock(2, 8, Color::LightBlue);
	GenerateBlock(3, 8, Color::Red);
	GenerateBlock(4, 8, Color::Green);
	GenerateBlock(5, 8, Color::Yellow);




}

void BlockManager::Uninit()
{
	for (auto& it : block)
	{
		it.Uninit();
	}

}

void BlockManager::Update()
{
	// ブロックを二次元配列へソートする
	SetSortBlocks();

	// ブロックが落下するかを判定する
	CheckDownBlock();

	for (auto& it : block)
	{
		it.Update();
	}
}

void BlockManager::Draw()
{
	sprBlock->Begin();
	for (auto& it : block)
	{
		if (9 <= it.GetColumn() || !it.GetIsExist())
		{
			continue;
		}

		sprBlock->Draw(it.GetPos().x + ADJUST + SINGLE_CORRECTION_X, it.GetPos().y + ADJUST + SINGLE_CORRECTION_Y, static_cast<float>(it.GetSizeX()), static_cast<float>(it.GetSizeY()), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, it.GetColor());
	}
	sprBlock->End();
}


void BlockManager::GenerateBlock(int _row, int _column, Color _color)
{
	for (auto& it : block)
	{
		if (it.GetIsExist()) continue;

		// If, GetIsExist is False.
		it.GenerateMe(_row, _column, _color);
		break;
	}
}

void BlockManager::SetStartBlock()
{
	// unknown
}

void BlockManager::BreakBlock(int _row, int _column)
{
	for (int i = 0; i < BLOCK_NUM_MAX; i++)
	{
		if (block[i].GetRow() == _row && block[i].GetColumn() == _column)
		{
			block[i].BreakMe();
			break;
		}
	}
}

void BlockManager::CheckDownBlock()
{
	for (int c = BOARD_COLUMN_MAX - 1; c > 0; --c) // 一番下は無条件で落ちないため -1
	{
		for (int r = 0;r<BOARD_ROW_MAX;r++)
		{
			if (sortBlocks[c + 1][r].GetColor() == -1 && !sortBlocks[c][r].GetIsFall() || sortBlocks[c+1][r].GetIsFall())
			{
				for (int i = 0; i < BLOCK_NUM_MAX; i++)
				{
					if (block[i].GetRow() == r && block[i].GetColumn() == c )
					{
						block[i].SetIsFall(true);
						break;
					}
				}

			}
		}
	}
}

void BlockManager::SetSortBlocks()
{
	Block dummy;
	for (int i = 0; i < COLUMN_MAX -10; i++) // COLUMN_MAX - お邪魔用の一盤面 + 下一列
	{
		for (int j = 0; j < ROW_MAX; j++)
		{
			sortBlocks[i][j] = dummy;
			for (int k = 0; k < BLOCK_NUM_MAX; k++)
			{
				if (block[k].GetRow() == j && block[k].GetColumn() == i)
				{
					sortBlocks[i][j] = block[k];
					break;
				}
			}
		}
	}
}
