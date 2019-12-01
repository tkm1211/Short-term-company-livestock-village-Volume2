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

}

void BlockManager::Uninit()
{

}

void BlockManager::Update()
{

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
